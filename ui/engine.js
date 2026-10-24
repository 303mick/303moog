/* REDLINE MODULAR audio engine (WebAudio).
   Listens to the UI only through the public window.Redline API (param / cable / note / wheel / panic events).
   The audio graph is built from the patch cables: if a cable is not patched, there is no sound path.
   Conventions: pitch CV is 1.0 per octave, 0 = C4 (MIDI 60). Gates and envelopes are 0..1. LFOs are +/-1. */
(function () {
  'use strict';
  const R = window.Redline;
  if (!R) { console.warn('engine: window.Redline missing'); return; }

  const AC = window.AudioContext || window.webkitAudioContext;
  let ctx = null, G = null;                    // G = current built graph
  const held = [];                             // note stack, last-note priority
  let bend = 0, modwheel = 0;
  const waveCache = new Map();

  const P = id => R.getState().params[id];
  const pv = (id, d = 0) => { const v = P(id); return v === undefined ? d : v; };
  const idx = (id, n) => Math.round(pv(id, 0) * (n - 1));
  const exp = (v, lo, hi) => lo * Math.pow(hi / lo, v);

  /* ---------- helpers ---------- */
  function track(n) { G.nodes.push(n); return n; }
  const gain = (v = 1) => { const g = track(ctx.createGain()); g.gain.value = v; return g; };
  const konst = (v = 0) => { const c = track(ctx.createConstantSource()); c.offset.value = v; c.start(); return c; };

  function periodic(shape, pw) {          // shape: 0 tri 1 shark 2 saw 3 ramp 4 square 5 pulse
    const key = shape + ':' + pw.toFixed(2);
    if (waveCache.has(key)) return waveCache.get(key);
    const f = p => {
      const saw = 2 * p - 1, tri = 4 * Math.abs(p - .5) - 1;
      switch (shape) { case 0: return tri; case 1: return (tri + saw) / 2; case 2: return saw; case 3: return -saw; case 4: return p < .5 ? 1 : -1; default: return p < pw ? 1 : -1; }
    };
    const N = 96, M = 2048, re = new Float32Array(N + 1), im = new Float32Array(N + 1);
    for (let h = 1; h <= N; h++) {
      let c = 0, sn = 0;
      for (let i = 0; i < M; i++) { const ph = (i + .5) / M, v = f(ph); c += v * Math.cos(2 * Math.PI * h * ph); sn += v * Math.sin(2 * Math.PI * h * ph); }
      re[h] = 2 * c / M; im[h] = 2 * sn / M;
    }
    const w = ctx.createPeriodicWave(re, im);
    waveCache.set(key, w);
    return w;
  }

  /* ---------- module builders: each returns { outs:{jack:node}, ins:{jack:node}, set:{param:fn} } ---------- */
  const B = {};

  B.kbd = id => {
    const pitch = konst(0), gate = konst(0), trig = konst(0), vel = konst(0), mod = konst(0), bendOut = konst(0);
    const set = {};
    G.kbd = { pitch, gate, trig, vel, mod, bendOut, id };
    G.applyPitch = (immediate) => {
      const top = held[held.length - 1];
      if (top === undefined) return;
      const oct = (idx(id + '.oct', 5) - 2);
      const range = [1, 2, 5, 12][idx(id + '.bend', 4)];
      const fine = (pv(id + '.tune', .5) - .5) * 2 * (50 / 1200);
      const target = (top - 60) / 12 + oct + fine + bend * range / 12;
      const tau = Math.max(.001, pv(id + '.glide', 0) * .6);
      const t = ctx.currentTime;
      pitch.offset.cancelScheduledValues(t);
      if (immediate || tau <= .002) pitch.offset.setValueAtTime(target, t);
      else pitch.offset.setTargetAtTime(target, t, tau / 3);
    };
    set.tune = set.bend = set.oct = set.glide = () => G.applyPitch(false);
    return { outs: { pitch, gate, trig, vel, mod, bend: bendOut }, ins: {}, set: Object.fromEntries(['glide', 'tune', 'oct', 'bend'].map(k => [k, set[k]])) };
  };

  B.osc = id => {
    const detune = gain(1);                       // sums cents from all CV inputs
    const base = konst(0);                        // range + frequency knob in cents
    base.connect(detune);
    const cv = gain(1200), fm = gain(240), pwm = gain(0);
    cv.connect(detune); fm.connect(detune);
    const outs = {}, oscs = [];
    const mk = (jack, shape) => {
      const o = track(ctx.createOscillator());
      o.frequency.value = 261.6256; o.start();
      detune.connect(o.detune);
      const lvl = gain(.5); o.connect(lvl);
      oscs.push({ o, jack, shape }); outs[jack] = lvl;
      return o;
    };
    mk('out', null); mk('saw', 2); mk('sqr', 4);
    const apply = () => {
      const sh = idx(id + '.wave', 6), pw = Math.min(.95, Math.max(.05, pv(id + '.pw', .5)));
      oscs.forEach(x => x.o.setPeriodicWave(periodic(x.shape === null ? sh : x.shape, x.shape === null ? pw : .5)));
      const rangeOct = [-6, -2, -1, 0, 1, 2][idx(id + '.range', 6)];
      base.offset.setTargetAtTime((rangeOct + (pv(id + '.freq', .5) - .5) * 2) * 1200, ctx.currentTime, .01);
    };
    apply();
    const s = { freq: apply, range: apply, wave: apply, pw: apply };
    return { outs, ins: { cv, fm, pwm }, set: s };
  };

  B.lfo = id => {
    const outs = {}, os = [];
    [['sin', 'sine'], ['tri', 'triangle'], ['sqr', 'square'], ['saw', 'sawtooth']].forEach(([j, t]) => {
      const o = track(ctx.createOscillator()); o.type = t; o.start(); const g = gain(1); o.connect(g); outs[j] = g; os.push(o);
    });
    const fmIn = gain(60);
    const apply = () => {
      const r = idx(id + '.range', 3), v = pv(id + '.freq', .35);
      const hz = [exp(v, .05, 2), exp(v, .5, 20), exp(v, 5, 200)][r];
      os.forEach(o => o.frequency.setTargetAtTime(hz, ctx.currentTime, .02));
    };
    apply(); os.forEach(o => fmIn.connect(o.frequency));
    return { outs, ins: { fm: fmIn }, set: { freq: apply, range: apply } };
  };

  B.sh = id => {
    const rnd = konst(0), out2 = gain(1); rnd.connect(out2);
    let timer = null;
    const loop = () => { rnd.offset.setTargetAtTime(Math.random() * 2 - 1, ctx.currentTime, Math.max(.001, pv(id + '.slew', .2) * .2));
      timer = setTimeout(loop, 1000 / exp(pv(id + '.rate', .4), .5, 40)); };
    loop(); G.timers.push(() => clearTimeout(timer));
    return { outs: { sh: rnd, rnd: out2 }, ins: {}, set: {} };
  };

  B.noise = id => {
    const len = ctx.sampleRate * 2, mkBuf = pink => {
      const b = ctx.createBuffer(1, len, ctx.sampleRate), d = b.getChannelData(0);
      let b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0;
      for (let i = 0; i < len; i++) {
        const w = Math.random() * 2 - 1;
        if (!pink) { d[i] = w * .5; continue; }
        b0 = .99886 * b0 + w * .0555179; b1 = .99332 * b1 + w * .0750759; b2 = .969 * b2 + w * .153852;
        b3 = .8665 * b3 + w * .3104856; b4 = .55 * b4 + w * .5329522; b5 = -.7616 * b5 - w * .016898;
        d[i] = (b0 + b1 + b2 + b3 + b4 + b5 + b6 + w * .5362) * .11; b6 = w * .115926;
      }
      return b;
    };
    const mk = pink => { const s = track(ctx.createBufferSource()); s.buffer = mkBuf(pink); s.loop = true; s.start(); const g = gain(1); s.connect(g); return g; };
    const w = mk(false), p = mk(true);
    const apply = () => { const l = pv(id + '.level', .7) * 1.2; w.gain.value = l; p.gain.value = l * 1.4; };
    apply();
    return { outs: { white: w, pink: p }, ins: {}, set: { level: apply, color: apply } };
  };

  B.env = id => {
    const out = konst(0), inv = gain(-1); out.connect(inv);
    const t = { a: v => exp(v, .002, 3), d: v => exp(v, .01, 6), r: v => exp(v, .01, 6) };
    const on = () => {
      const T = ctx.currentTime, a = t.a(pv(id + '.attack')), d = t.d(pv(id + '.decay')), s = pv(id + '.sustain');
      out.offset.cancelScheduledValues(T); out.offset.setValueAtTime(out.offset.value, T);
      out.offset.linearRampToValueAtTime(1, T + a);
      out.offset.setTargetAtTime(s, T + a, d / 3);
    };
    const off = () => {
      const T = ctx.currentTime; out.offset.cancelScheduledValues(T); out.offset.setValueAtTime(out.offset.value, T);
      out.offset.setTargetAtTime(0, T, t.r(pv(id + '.release')) / 3);
    };
    G.envs.push({ id, on, off, gateJack: id + '.gate' });
    return { outs: { out, inv }, ins: { gate: gain(0) }, set: {} };
  };

  B.filt = (id, type) => {
    const stages = [track(ctx.createBiquadFilter()), track(ctx.createBiquadFilter())];
    stages.forEach(f => f.type = type);
    const inG = gain(1), outG = gain(1);
    const cvAmt = gain(1), cv1 = gain(1), cv2 = gain(1);
    cv1.connect(cvAmt); cv2.connect(cvAmt);
    stages.forEach(f => cvAmt.connect(f.detune));
    const apply = () => {
      const isBp = type === 'bandpass', v = pv(id + (isBp ? '.center' : '.cutoff'), .5);
      const hz = exp(v, 20, 18000), T = ctx.currentTime;
      stages.forEach(f => {
        f.frequency.setTargetAtTime(hz, T, .01);
        f.Q.setTargetAtTime(isBp ? 12 - pv(id + '.bw', .4) * 11.3 : -3 + pv(id + '.emph', .2) * 24, T, .01);
      });
      cvAmt.gain.value = pv(id + '.amt', .5) * 3600;
      inG.disconnect(); stages[0].disconnect(); stages[1].disconnect();
      const two = id === 'lpf' && idx('lpf.slope', 2) === 1;
      inG.connect(stages[0]);
      if (two) { stages[0].connect(stages[1]); stages[1].connect(outG); } else stages[0].connect(outG);
    };
    apply();
    return { outs: { out: outG }, ins: { in: inG, cv1, cv2, cv: cv1 }, set: { cutoff: apply, emph: apply, amt: apply, slope: apply, center: apply, bw: apply } };
  };

  B.mix = id => {
    const out = gain(1), inv = gain(-1); out.connect(inv);
    const ins = {}, chans = [1, 2, 3, 4].map(n => { const g = gain(0); g.connect(out); ins['in' + n] = g; return g; });
    const apply = () => chans.forEach((g, i) => g.gain.setTargetAtTime(pv(id + '.l' + (i + 1), .5) * 1.0, ctx.currentTime, .01));
    apply();
    return { outs: { out, inv }, ins, set: { l1: apply, l2: apply, l3: apply, l4: apply } };
  };

  B.vca = id => {
    const amp = gain(0), outG = gain(1), in1 = gain(1), in2 = gain(1);
    in1.connect(amp); in2.connect(amp); amp.connect(outG);
    const cvG = gain(1); cvG.connect(amp.gain);
    const cv1 = gain(1), cv2 = gain(1); cv1.connect(cvG); cv2.connect(cvG);
    const apply = () => { amp.gain.setTargetAtTime(pv(id + '.bias', 0), ctx.currentTime, .01); cvG.gain.value = pv(id + '.amt', .8); outG.gain.setTargetAtTime(pv(id + '.gain', .7) * 1.4, ctx.currentTime, .01); };
    apply();
    return { outs: { out: outG }, ins: { in1, in2, cv1, cv2 }, set: { bias: apply, amt: apply, gain: apply } };
  };

  B.att = id => {
    const outs = {}, ins = {}, gs = [1, 2, 3, 4].map(n => { const i = gain(1), o = gain(1); i.connect(o); ins['in' + n] = i; outs['out' + n] = o; return o; });
    const apply = () => gs.forEach((g, i) => g.gain.setTargetAtTime(pv(id + '.a' + (i + 1), .5) * 2, ctx.currentTime, .01));
    apply();
    return { outs, ins, set: { a1: apply, a2: apply, a3: apply, a4: apply } };
  };

  B.mult = () => {
    const ins = { ina: gain(1), inb: gain(1) }, outs = {};
    [1, 2, 3].forEach(n => { const a = gain(1), b = gain(1); ins.ina.connect(a); ins.inb.connect(b); outs['a' + n] = a; outs['b' + n] = b; });
    return { outs, ins, set: {} };
  };

  B.delay = id => {
    const inG = gain(1), outG = gain(1), dry = gain(.7), wet = gain(.3), dl = track(ctx.createDelay(1.5)), fb = gain(.35), tone = track(ctx.createBiquadFilter());
    tone.type = 'lowpass';
    inG.connect(dry); dry.connect(outG); inG.connect(dl); dl.connect(tone); tone.connect(fb); fb.connect(dl); tone.connect(wet); wet.connect(outG);
    const apply = () => { const T = ctx.currentTime, m = pv(id + '.mix', .3);
      dl.delayTime.setTargetAtTime(.03 + .87 * Math.pow(pv(id + '.time', .4), 2), T, .05);
      fb.gain.setTargetAtTime(pv(id + '.fb', .35) * .92, T, .02); tone.frequency.setTargetAtTime(exp(pv(id + '.tone', .5), 400, 14000), T, .02);
      wet.gain.setTargetAtTime(m, T, .02); dry.gain.setTargetAtTime(1 - m * .6, T, .02); };
    apply();
    return { outs: { out: outG }, ins: { in: inG, tcv: gain(0) }, set: { time: apply, fb: apply, tone: apply, mix: apply } };
  };

  B.phaser = id => {
    const inG = gain(1), outG = gain(1), dry = gain(1), wet = gain(.4);
    const aps = [0, 1, 2, 3].map(() => { const f = track(ctx.createBiquadFilter()); f.type = 'allpass'; f.frequency.value = 800; return f; });
    inG.connect(dry); dry.connect(outG); let prev = inG; aps.forEach(f => { prev.connect(f); prev = f; }); prev.connect(wet); wet.connect(outG);
    const lfo = track(ctx.createOscillator()), depth = gain(1200); lfo.type = 'sine'; lfo.start(); lfo.connect(depth); aps.forEach(f => depth.connect(f.detune));
    const apply = () => { lfo.frequency.setTargetAtTime(exp(pv(id + '.rate', .3), .05, 8), ctx.currentTime, .05); depth.gain.value = pv(id + '.depth', .5) * 2400; wet.gain.value = pv(id + '.mix', .4); };
    apply();
    return { outs: { out: outG }, ins: { in: inG, rcv: gain(0) }, set: { rate: apply, depth: apply, mix: apply, fb: apply } };
  };

  B.reverb = id => {
    const inG = gain(1), outG = gain(1), dry = gain(.7), wet = gain(.3), conv = track(ctx.createConvolver()), damp = track(ctx.createBiquadFilter());
    damp.type = 'lowpass';
    const ir = () => {
      const secs = .4 + pv(id + '.size', .5) * 3 * (.5 + pv(id + '.decay', .5)), len = Math.floor(ctx.sampleRate * secs), b = ctx.createBuffer(2, len, ctx.sampleRate);
      for (let c = 0; c < 2; c++) { const d = b.getChannelData(c); for (let i = 0; i < len; i++) d[i] = (Math.random() * 2 - 1) * Math.pow(1 - i / len, 2.6); }
      conv.buffer = b;
    };
    ir(); inG.connect(dry); dry.connect(outG); inG.connect(conv); conv.connect(damp); damp.connect(wet); wet.connect(outG);
    let deb = null;
    const live = () => { const m = pv(id + '.mix', .3); wet.gain.setTargetAtTime(m * 1.5, ctx.currentTime, .02); dry.gain.setTargetAtTime(1 - m * .5, ctx.currentTime, .02); damp.frequency.setTargetAtTime(exp(1 - pv(id + '.damp', .4), 800, 16000), ctx.currentTime, .02); };
    live();
    const rebuild = () => { live(); clearTimeout(deb); deb = setTimeout(() => { if (G && G.alive) ir(); }, 250); };
    return { outs: { out: outG }, ins: { in: inG, mcv: gain(0) }, set: { size: rebuild, decay: rebuild, damp: live, mix: live } };
  };

  B.main = id => {
    const inl = gain(1), inr = gain(1), vol = gain(.7), merger = track(ctx.createChannelMerger(2)), limiter = track(ctx.createDynamicsCompressor()), an = track(ctx.createAnalyser());
    an.fftSize = 2048;
    const pl = gain(1), pr = gain(1);
    inl.connect(pl); inr.connect(pr); pl.connect(merger, 0, 0); pr.connect(merger, 0, 1);
    merger.connect(vol); vol.connect(limiter); limiter.connect(ctx.destination); limiter.connect(an);
    limiter.threshold.value = -6; limiter.ratio.value = 12; limiter.attack.value = .003; limiter.release.value = .1;
    G.analyser = an;
    const apply = () => { const T = ctx.currentTime, b = pv(id + '.pan', .5) * 2 - 1;
      vol.gain.setTargetAtTime(Math.pow(pv(id + '.vol', .7), 2) * 1.6, T, .01); pl.gain.setTargetAtTime(b > 0 ? 1 - b : 1, T, .01); pr.gain.setTargetAtTime(b < 0 ? 1 + b : 1, T, .01); };
    apply();
    return { outs: {}, ins: { inl, inr }, set: { vol: apply, pan: apply } };
  };

  const MAKERS = (id) => {
    if (id === 'kbd') return B.kbd(id);
    if (/^osc/.test(id)) return B.osc(id);
    if (id === 'lfo1') return B.lfo(id);
    if (id === 'lfo2') return B.sh(id);
    if (id === 'noise') return B.noise(id);
    if (/^env/.test(id)) return B.env(id);
    if (id === 'lpf') return B.filt(id, 'lowpass');
    if (id === 'hpf') return B.filt(id, 'highpass');
    if (id === 'bpf') return B.filt(id, 'bandpass');
    if (/^mix/.test(id)) return B.mix(id);
    if (/^vca/.test(id)) return B.vca(id);
    if (/^att/.test(id)) return B.att(id);
    if (id === 'mult') return B.mult(id);
    if (id === 'delay') return B.delay(id);
    if (id === 'phaser') return B.phaser(id);
    if (id === 'reverb') return B.reverb(id);
    if (id === 'main') return B.main(id);
    return null;
  };

  /* ---------- build / teardown ---------- */
  function teardown() {
    if (!G) return;
    G.alive = false; G.timers.forEach(f => f());
    G.nodes.forEach(n => { try { if (n.stop) n.stop(); } catch (e) {} try { n.disconnect(); } catch (e) {} });
    G = null;
  }

  function build() {
    if (!ctx) return;
    teardown();
    G = { nodes: [], mods: {}, timers: [], envs: [], alive: true, kbd: null };
    const ids = [...R.modules.top, ...R.modules.bottom];
    ids.forEach(id => { try { G.mods[id] = MAKERS(id); } catch (e) { console.error('engine build', id, e); } });
    const st = R.getState();
    // gate sources: an envelope follows the keyboard only if its gate jack is patched from kbd.gate / kbd.trig
    G.envs.forEach(e => { e.follow = st.cables.some(c => c.to === e.gateJack && /^kbd\.(gate|trig)$/.test(c.from)); });
    st.cables.forEach(c => {
      const [fm, fj] = c.from.split('.'), [tm, tj] = c.to.split('.');
      const src = G.mods[fm] && G.mods[fm].outs[fj], dst = G.mods[tm] && G.mods[tm].ins[tj];
      if (src && dst && tj !== 'gate') { try { src.connect(dst); } catch (e) { console.warn('cable', c, e.message); } }
    });
    if (G.applyPitch && held.length) G.applyPitch(true);
    if (held.length && G.kbd) { G.kbd.gate.offset.value = 1; G.envs.forEach(e => e.follow && e.on()); }
    G.kbd && (G.kbd.mod.offset.value = modwheel, G.kbd.bendOut.offset.value = bend);
  }

  /* ---------- events from the UI ---------- */
  function ensure() {
    if (!AC) return false;
    if (!ctx) { ctx = new AC({ latencyHint: 'interactive' }); build(); badge(); }
    if (ctx.state === 'suspended') ctx.resume().then(badge);
    return true;
  }
  function noteOn(n, vel) {
    if (!ensure() || !G) return;
    const i = held.indexOf(n); if (i >= 0) held.splice(i, 1); held.push(n);
    const retrig = held.length === 1 || true;
    G.applyPitch && G.applyPitch(held.length === 1);
    if (G.kbd) { const t = ctx.currentTime; G.kbd.gate.offset.setValueAtTime(1, t); G.kbd.vel.offset.setValueAtTime(vel / 127, t);
      G.kbd.trig.offset.setValueAtTime(1, t); G.kbd.trig.offset.setValueAtTime(0, t + .01); }
    if (retrig) G.envs.forEach(e => e.follow && e.on());
  }
  function noteOff(n) {
    const i = held.indexOf(n); if (i < 0) return; held.splice(i, 1);
    if (!G) return;
    if (held.length) { G.applyPitch && G.applyPitch(false); return; }
    G.kbd && G.kbd.gate.offset.setValueAtTime(0, ctx.currentTime);
    G.envs.forEach(e => e.follow && e.off());
  }

  R.on('note', e => e.on ? noteOn(e.note, e.velocity || 100) : noteOff(e.note));
  R.on('panic', () => { held.length = 0; if (G) { G.kbd && G.kbd.gate.offset.setValueAtTime(0, ctx.currentTime); G.envs.forEach(e => e.off()); } });
  R.on('wheel', e => {
    if (e.id === 'bend') bend = e.value; else modwheel = e.value;
    if (!G || !G.kbd) return;
    if (e.id === 'bend') { G.kbd.bendOut.offset.value = bend; G.applyPitch && held.length && G.applyPitch(false); } else G.kbd.mod.offset.value = modwheel;
  });
  R.on('cable', () => { clearTimeout(build.t); build.t = setTimeout(build, 30); });
  R.on('param', e => {
    if (!G) return;
    const [m, p] = e.id.split('.'), mod = G.mods[m];
    if (mod && mod.set[p]) { try { mod.set[p](); } catch (err) { console.error(err); } }
    if (/^env\d\.(attack|decay|sustain|release)$/.test(e.id)) return;
  });

  /* ---------- VU meter + start-up badge ---------- */
  const buf = new Float32Array(2048);
  setInterval(() => {
    if (!G || !G.analyser || !R.setVU) return;
    G.analyser.getFloatTimeDomainData(buf);
    let s = 0; for (let i = 0; i < buf.length; i++) s += buf[i] * buf[i];
    const rms = Math.sqrt(s / buf.length), db = 20 * Math.log10(rms + 1e-6);
    R.setVU(Math.max(0, Math.min(6, Math.round((db + 54) / 9))));
  }, 80);

  let badgeEl = null;
  function badge() {
    if (!badgeEl) {
      badgeEl = document.createElement('div');
      badgeEl.setAttribute('role', 'status');
      badgeEl.style.cssText = 'position:fixed;left:12px;bottom:10px;z-index:99;padding:6px 10px;border-radius:4px;font:700 12px Arial,sans-serif;letter-spacing:.06em;color:#f7f3e5;background:rgba(120,20,16,.92);border:1px solid #000;pointer-events:none';
      document.body.appendChild(badgeEl);
    }
    const on = ctx && ctx.state === 'running';
    badgeEl.textContent = on ? 'SOUND ON 2022 keys: A S D F G H J K L (W E T Y U O P black) 2022 Z X C V B N M lower' : 'CLICK ANYWHERE OR PRESS A KEY FOR SOUND';
    badgeEl.style.background = on ? 'rgba(20,90,30,.92)' : 'rgba(120,20,16,.92)';
    if (on) setTimeout(() => { if (badgeEl) badgeEl.style.opacity = '0'; }, 6000);
  }
  ['pointerdown', 'keydown'].forEach(ev => window.addEventListener(ev, () => ensure(), { capture: true }));
  document.addEventListener('DOMContentLoaded', badge); if (document.readyState !== 'loading') badge();

  R.engine = { get ctx() { return ctx; }, get analyser() { return G && G.analyser; }, rebuild: build, ensure };
})();
