# XIAO ESP32-C5 FastLED 單獨測試

這是一個獨立的 PlatformIO 專案，只使用 FastLED 控制 D1 上的第一顆 WS2812B；不啟動 WiFi、OLED 或原專案的 debug mode。

## 接線

- XIAO **D1** → WS2812B 第一顆的 **DIN**（確認燈條箭頭方向）
- 燈條 **VCC** → 5 V 電源
- 燈條 **GND** → 電源 GND，且與 XIAO **GND 共地**
- 測試期間可先移除 D1 到 D0 的量測跳線。

## 使用

在 VS Code 用「檔案 → 開啟資料夾」開啟本資料夾 `fastled-only`，再用 PlatformIO 的 Upload。開啟 115200 baud 的 Serial Monitor，必要時按一下板子的 Reset。

第一顆燈預期每秒依序顯示 **紅、綠、藍、白、熄滅**，持續循環。序列埠每次會顯示 `before show: ...` 和 `after show: ...`；後者只表示 `FastLED.show()` 已返回，不能單獨證明資料波形正確。

如果燈仍不亮，5 V 供電的 WS2812B 可能無法可靠辨識 XIAO 的 3.3 V 資料訊號，可再測試 3.3 V → 5 V 電位轉換器。

若序列埠出現 `parlio_tx_create_trans_queue: no mem for queue`，表示 PARLIO 驅動沒有成功建立傳送佇列。此專案已在 `platformio.ini` 停用 PARLIO，讓 FastLED 選用 RMT；修改設定後需重新 Build 與 Upload 才會生效。
