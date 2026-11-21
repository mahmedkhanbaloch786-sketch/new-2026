# Beauty Skin Retouching — Update Specification (v0.3)

## UI direction
A colorful, professional, Magic-Bullet-Looks-inspired full-page Look workspace. The interface is a parameter-driven OFX effect, not a LUT browser or lookup-table editor. Keep six functional pages separated; do not flatten all controls into one page.

## Six pages
1. **Basic** — Saturation, Sharpness, Detail, Contrast, Exposure, Global Blend.
2. **RGB Matrix** — 3×3 matrix with nine independent signed controls and Matrix Blend.
3. **Color** — Red, Green, Blue, Cyan, Magenta, Yellow and Global Saturation.
4. **Beauty & Selection** — Dark, White, Black, Red, Hue and Luma selection controls; subject/sky selection and mask preview.
5. **Skin Retouching** — Sharpness, Detail, Blur, Softness and Retouch Strength.
6. **Advanced** — fine tuning, per-channel gain/offset, hue, luma and Global Blend.

## Cinematic Look catalog
The update notes specify **15 total looks**, and explicitly name these six new looks:
- Blockbuster Orange
- Wedding Soft Pastel
- Kodak Warm Film
- Matte Black
- Cyber Teal-Magenta
- Cold Thriller

The nine earlier looks are not named in the supplied update notes; keep their existing slots/names from the prior build rather than inventing replacements.

## S-Log3 to Rec.709 converter
Seven entries total:
1. S-Log3 > Rec.709 (Neutral) — conversion only, no creative style.
2. Warm
3. Teal & Orange
4. Film Print
5. Moody
6. Clean & Bright
7. Soft Pastel

Additional controls, only for S-Log3 looks:
- S-Log3 Gamut: S-Gamut3.Cine (default) or S-Gamut3.
- S-Log3 Exposure: approximately −2 to +2 stops.

Conversion must precede creative looks, skin retouching and AI selection. Look Amount changes creative style intensity only; it must not reduce the technical S-Log3 conversion.

## AI selection fixes from the supplied PDF
- Flip incoming frame rows for analysis because OpenFX row order is bottom-to-top; flip the resulting mask back for output.
- Apply the same coordinate correction to Sky and Subject selection.
- Do not count red clothing/hair touching the frame border as background color.
- Do not reject a close-up face merely because it touches the lower frame edge; continue rejecting ground/wall-like surfaces.
- The supplied notes report a test frame selection of about 29% and landscape scores Sky 0.997 / Subject 0.985. These are reported test results, not a guarantee for every frame.
- If a selection looks too narrow, first set Selection Grow-Shrink to 0; then increase Body Reach if required.

## Important technical distinction
The current C++ source is a CPU-rendering OFX starter implementation and includes basic controls/processing. It is **not yet a verified production build** of every item above. A real host-tested release still requires the official OpenFX SDK, Windows x64 compiler, and validation in the user's EDIUS 11 + OFX Bridge installation. The notes' S-Log3 values (18% gray at 0.409, black at 0, no gray color cast, highlight roll-off) are acceptance tests to run; do not advertise them as passed by this source package.
