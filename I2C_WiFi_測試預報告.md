# I²C 與 Wi‑Fi 測試預報告

日期：2026-11-18  
依據：目前的 `src/main.cpp`、`platformio.ini`。以下是程式碼推導的預期現象，尚未實測。

## 測試設定

| 項目 | 程式設定 |
| --- | --- |
| 板子 | Seeed XIAO ESP32-C5，Arduino／PlatformIO |
| 序列埠 | 115200 baud |
| I²C | `SDA_PIN = 23`、`SCL_PIN = 24`；SSD1306 OLED，128 × 64，位址 `0x3C` |
| 開機模式 | `DEBUG_PIN = 9`，上拉；接地為除錯模式，否則為一般模式 |
| Wi‑Fi 選擇 | `WIFI_PIN = 11`，上拉；高電位選 `EE219B`，接地選 `Lightdance` |
| 資料來源 | `SWITCH_PIN = 10`，上拉；接地讀取 LittleFS，否則連網下載 |

三個模式腳位都只在 `setup()` 讀取；改變接線後須重新開機，才會切換路徑。

## I²C／OLED：預期看到的現象

1. 開機後，程式先設定 I²C 腳位並啟動 `Wire`，接著初始化位址 `0x3C` 的 OLED。
2. OLED 應先顯示 `Starting...`。序列埠也應印出同樣文字。
3. 一般模式會再依 Wi‑Fi 及資料來源顯示下表的訊息。`msg()` 同時更新 OLED 和序列埠，但相同訊息不會重複印出。

**判讀界線：**程式沒有 I²C 位址掃描，也沒有檢查 `oled.begin()` 的回傳值。序列埠出現 `Starting...` 只能證明程式執行到顯示呼叫，不能單獨證明 OLED 或 I²C 通訊成功。若序列埠有文字而 OLED 全黑，需先核對電源、SDA／SCL 接線、`0x3C` 位址及板上可用腳位；目前程式本身不會顯示具體 I²C 錯誤。

## Wi‑Fi：預期看到的現象

測 Wi‑Fi 時，`DEBUG_PIN` 保持高電位。若接地，程式會進入除錯模式，不執行 Wi‑Fi 連線。

| `WIFI_PIN` 開機狀態 | 預期 OLED／序列埠訊息 | 連上後的 IP |
| --- | --- | --- |
| 高電位 | `WiFi: EE219B` → `Connected` 與 IP | 由 DHCP 指派，實際數值不固定 |
| 接地 | `WiFi: Lightdance` → `Connected` 與 IP | 程式設定為 `192.168.1.100`（`PLAYER_NUM = 0`） |

程式最多輪詢連線狀態 10 次、每次間隔 500 ms。若仍未連上，顯示 `WiFi failed`，約 2 秒後重新開機，然後再次出現 `Starting...`。這個循環代表連線未成功；`Connected` 代表 Wi‑Fi 已連上並取得／設定本機 IP，但不代表後續 HTTPS 或 UDP 已成功。

### `SWITCH_PIN` 接地：讀取已儲存資料

連線後預期依序顯示 `Loading...`、短暫的 `Ready!` 加 IP，接著顯示 `Ready!`。程式會在 UDP 12345 埠等待封包；若約 5 秒沒有任何 UDP 封包，會顯示 `No heartbeat`／`Reboot` 並重新開機。因此，持續看到 `Ready!` 需要外部主機定期送封包。

這條路徑沒有確認 `/data.bin` 存在或資料有效；看到 `Ready!` 不能當作動畫資料已正確讀取的證據。

### `SWITCH_PIN` 高電位：下載資料

連線後預期顯示 `Downloading...`，然後依序顯示 `Chunk 0` 到 `Chunk 59`。單個 chunk 失敗時會顯示 `Retry 1/3` 等訊息；三次都失敗則停在 `Download failed`。全部成功後儲存資料並停在 `Download success`，不會進入 `Ready!` 或 UDP 收訊階段。若今天只想看 Wi‑Fi 是否連上，`Connected` 和其 IP 是此路徑最直接的觀察點。

## 現場紀錄欄

| 檢查項目 | 預期 | 實際觀察 |
| --- | --- | --- |
| OLED 開機畫面 | `Starting...` 清楚顯示 | |
| 序列埠 | 115200 baud 印出與畫面相同的狀態 | |
| Wi‑Fi 設定選擇 | 顯示所選 SSID | |
| Wi‑Fi 連線 | 顯示 `Connected` 與 IP，或 `WiFi failed` 後重開 | |
| 資料路徑 | 讀取時顯示 `Loading...`／`Ready!`；下載時顯示 chunk 結果 | |

## 程式碼上的注意點

- `README.md` 仍描述「內建 LED 每秒切換」的起始範例，與目前程式不符；本報告以 `src/main.cpp` 為準。
- 原始碼內的 Pico W／RP2350／CYW43 註解是舊平台描述；目前 `platformio.ini` 的目標板為 XIAO ESP32-C5。
- 程式沒有獨立的 I²C 測試輸出；本次 I²C 的可見指標是 OLED 能否正確顯示狀態。
