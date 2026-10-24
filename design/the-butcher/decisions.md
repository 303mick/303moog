# Decisions: The Butcher

## 2026-10-24: Minimum window size is 968 x 598, not 880 x 544
The brief set the minimum at 880 x 544 (0.8x). At that scale the 11 px legends would render at 8.8 px, below the 10 px floor. The minimum is now 968 x 598 (0.88x), where legends render at 9.7 px. Default (1100 x 680) and maximum (1650 x 1020) are unchanged. Applied in `ux.md` sections 3.5, 4.2 and 7 and assumption 6.
