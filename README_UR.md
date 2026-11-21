# Beauty Skin Retouching — ColorSuite OFX v0.3

یہ EDIUS 11 / OFX Bridge کے لیے Windows x64 OFX plugin کا source project ہے۔ اس میں سابقہ ColorSuite C++ starter، CMake build، Inno Setup installer script، اور Beauty Skin Retouching update کی مکمل UI/feature specification شامل ہے۔

**ڈیزائن:** Magic Bullet Looks سے متاثر مکمل colorful Look workspace؛ LUT Lookup Table جیسا workflow نہیں۔ چھ الگ pages: Basic، RGB Matrix، Color، Beauty & Selection، Skin Retouching، Advanced۔

**PDF کے مطابق شامل specification:** 15 Looks (چھ نئے نام واضح طور پر درج)، S-Log3 سے Rec.709 کے سات انداز، S-Gamut3.Cine/S-Gamut3، ±2 stops exposure، اور AI selection کے frame-row orientation fixes۔ تفصیل `docs/` میں ہے۔

## اہم بات
یہ ZIP source/build package ہے، تیار شدہ `.exe` یا verified `.ofx` binary نہیں۔ اس ماحول میں Windows x64 compiler، OpenFX SDK اور Inno Setup موجود نہیں، اس لیے حقیقی Windows installer compile یا EDIUS 11 میں test ہونے کا دعویٰ نہیں کیا جا سکتا۔ Build steps `BUILD_WINDOWS_UR.txt` میں ہیں۔ موجودہ C++ starter کو full Look UI، S-Log3 converter اور PDF والے AI selection fixes کے ساتھ مکمل اور host-test کرنا ابھی باقی ہے۔
