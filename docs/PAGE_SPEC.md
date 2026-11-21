# UI / Parameter specification

## Page 1 — Basic Adjustments
- Saturation: 0–2
- Sharpness: -1–1
- Detail: -1–1
- Contrast: 0–2
- Exposure: -4–4 stops
- Global Blend: 0–1

## Page 2 — Color Matrix
- Red-Red, Red-Green, Red-Blue
- Green-Red, Green-Green, Green-Blue
- Blue-Red, Blue-Green, Blue-Blue
- Matrix Blend
- All matrix controls: -2–2

## Page 3 — Color Saturation
- Red, Green, Blue, Cyan, Magenta, Yellow Saturation
- Global Saturation
- Each sector: 0–3

## Page 4 — Beauty & Color Selection
- Hue Center / Hue Width
- Saturation Minimum / Maximum
- Luma Minimum / Maximum
- Dark / White / Black / Red selection strengths
- Show Selection Mask

## Page 5 — Skin Retouching
- Skin Range
- Skin Sharpness / Skin Detail
- Blur / Softness
- Retouch Strength

## Page 6 — Advanced Controls
- Red / Green / Blue Gain
- Red / Green / Blue Offset
- Hue Rotation
- Luma
- Global Blend

## Host UI note
The source currently puts the page name into parameter hints. True native tab/page grouping depends on the OFX host and bridge. Update the parameter grouping metadata after checking the exact bridge implementation.
