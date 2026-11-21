# Full-page Look workspace — UI layout

## Visual system
- Dark graphite canvas with colorful cinematic accent colors and restrained gradients.
- Large preview area on the left; Look browser and parameter controls on the right.
- Top navigation: **Looks | S-Log3 Convert | Basic | RGB Matrix | Color | Beauty & Selection | Skin | Advanced**.
- Large Look tiles with a clear selected state, but no LUT-table grid, LUT-file picker, or `.cube` workflow.
- Look Amount slider is separate from the S-Log3 conversion enable/state.
- Keep each of the six control pages independent and scrollable.

## Look page
- Header: `CINEMATIC LOOKS`
- Look cards: 15 total slots. Six newly named cards are listed in `BEAUTY_SKIN_RET0UCHING_UPDATE_UR.md`; preserve the nine existing legacy names from the previous build.
- Controls: Look Amount (0–100%), Reset Look, Bypass Look.

## S-Log3 page
- Conversion dropdown: Neutral, Warm, Teal & Orange, Film Print, Moody, Clean & Bright, Soft Pastel.
- Gamut dropdown: S-Gamut3.Cine / S-Gamut3.
- Exposure: −2.0 to +2.0 stops.
- Conversion happens before the creative style and remains full-strength when Look Amount is reduced.

## Beauty & Selection page
- Selection mode: Subject / Sky / Skin / Manual color range.
- Show Mask toggle.
- Selection Grow-Shrink and Body Reach controls.
- Hue, Saturation and Luma range controls.
