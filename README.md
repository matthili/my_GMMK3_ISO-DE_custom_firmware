# 🎹 GMMK3 P100 ISO-DE Custom QMK Firmware

> **Eine persönliche Firmware-Adaption für die Glorious GMMK3 100% (P100) mit deutschem ISO-Layout.**
>
> Portiert von der bewährten [GMMK2 ISO-DE Custom Firmware](https://github.com/matthili/my_GMMK2_ISO-DE_custom_firmware), angepasst an die Hardware der GMMK3 P100 – einschließlich Drehregler (Rotary Encoder) und Seitenbeleuchtung.

---

## ⬇️ Download & Flash

### Firmware-Datei

| Modell | Download |
|---|---|
| **GMMK3 P100 ISO** | [`gmmk_gmmk3_p100_iso_iso_de_custom.bin`](firmware/gmmk_gmmk3_p100_iso_iso_de_custom.bin) |

> ⚠️ **Achtung:** Nur für die **ISO**-Variante der GMMK3 P100. Für ANSI-Layouts ist diese Firmware nicht geeignet.

### Flashen mit QMK Toolbox

1. **[QMK Toolbox herunterladen](https://github.com/qmk/qmk_toolbox/releases)** und starten
2. Oben rechts die passende `.bin`-Datei laden (**Open** → Datei auswählen)
3. Tastatur in den **Bootloader-Modus** versetzen – je nach Hardware-Revision gibt es zwei Methoden:

   **Methode A – Tastatur bereits angesteckt:**
   - **FN + ESC** drücken
   - Alternativ: Reset-Taster auf der Rückseite der Tastatur drücken

   **Methode B – Tastatur noch nicht angesteckt (Bootmagic):**
   - **ESC** gedrückt halten (die ESC-Taste ist als Bootmagic-Key definiert)
   - Tastatur dabei per USB anstecken
   - Taste erst loslassen, wenn QMK Toolbox die Verbindung meldet

   > ℹ️ Wenn **FN + ESC** keine Reaktion zeigt, ist Methode B die richtige Wahl.

4. QMK Toolbox zeigt im Log: `WB32 DFU device connected`
5. **Flash** klicken – fertig!

> 💡 **Beim ersten Flash nach einem früheren VIA-Einsatz:** Halte beim Einstecken der Tastatur die **ESC-Taste gedrückt** – das löscht den EEPROM und stellt sicher, dass die neue Tastenbelegung korrekt übernommen wird.

---

## ✨ Features

### 🔧 Key Chatter Fix (Debounce = 25 ms)
Viele Schalter prellen bis zu 20 ms und verursachen dabei Doppeleingaben. Der QMK-Standard liegt bei nur 5 ms. Dieser Wert wurde auf **25 ms** erhöht, um auch stark prellende Schalter sicher abzufangen.

### 🇦🇹/🇩🇪 Deutsches ISO-Layout (QWERTZ)
- Vollständige QWERTZ-Tastenbelegung für AT/DE
- Umlaute direkt auf den gewohnten Positionen: **Ö, Ä, Ü, ß**
- Sonderzeichen korrekt: **^, <, #, +, ´**
- Z und Y korrekt vertauscht gegenüber QWERTY

### 🎛️ VIA-Unterstützung
- VIA-Support ist aktiviert (`VIA_ENABLE = yes`)
- Tastenänderungen können jederzeit per Browser über **[usevia.app](https://usevia.app)** vorgenommen werden, ohne neu zu kompilieren
- 4 Keymap-Layer sind im EEPROM gespeichert (Win-Base, Win-FN, Mac-Base, Mac-FN)

### 🔊 Drehregler (Rotary Encoder)
- **Drehen:** Lautstärke hoch / runter
- **Drücken:** Mute (Ton stumm schalten)
- **FN + Drücken:** Seiten-LEDs an/aus umschalten

### 🔀 DIP-Switch (Rückseite)
- **Position 1 (aktiv):** Windows-Layer (Standard)
- **Position 2:** Mac OS-Layer (Cmd/Opt statt Win/Alt)

### 💡 Anpassbare Hintergrundbeleuchtung

#### Basisfarbe (10 Presets, persistent gespeichert)
Über **FN + →** und **FN + ←** wird zwischen 10 Basisfarben gewechselt. Die gewählte Farbe bleibt nach dem Ausschalten erhalten.

| # | Farbe | RGB |
|---|---|---|
| 0 | 🔵 Blau (Standard) | `0, 0, 255` |
| 1 | 🟡 Gelb | `255, 200, 0` |
| 2 | 🟣 Violett | `148, 0, 211` |
| 3 | 🌸 Rosa | `255, 20, 147` |
| 4 | 🩵 Türkis | `0, 200, 200` |
| 5 | 🟠 Orange | `255, 80, 0` |
| 6 | 🟢 Hellgrün | `50, 255, 50` |
| 7 | ⚪ Weiß | `255, 255, 255` |
| 8 | 🔴 Rot | `255, 0, 0` |
| 9 | ⚫ Aus | `0, 0, 0` *(Reaktiv-Effekte bleiben aktiv)* |

#### Helligkeit
**FN + ↑** und **FN + ↓** steuern die globale Helligkeit. Wird ebenfalls persistent gespeichert.

#### Seitenbeleuchtung (Side LEDs)
Die 20 Seiten-LEDs (je 10 links und rechts) leuchten immer in der aktuell gewählten Basisfarbe und gleichen Helligkeit. Mit **FN + Encoder-Taste** (Drehregler drücken) können sie ein- und ausgeschaltet werden. Dieser Zustand wird im EEPROM gespeichert und bleibt nach dem Ausschalten erhalten.

### 🌊 Reaktive RGB-Effekte

| Taste | Effekt |
|---|---|
| **ESC** | Roter Wellenring, der sich radial über die gesamte Tastatur ausbreitet |
| **ENTER** | Identischer roter Welleneffekt |
| **Numpad ENTER** | Identischer roter Welleneffekt |
| **Alle anderen Tasten** | Kurzer grüner Flash auf der gedrückten Taste (500 ms Fade-Out) |

> Die Reaktiv-Effekte sind *immer* aktiv – auch im Basisfarben-Modus „Aus" (Index 9).

### 🔴🟢 Lock-Tasten Puls-Indikatoren

Die Lock-Tasten signalisieren Zustandswechsel mit einer Puls-Animation (5× pulsierend über 3 Sekunden).

> **Normalzustand:** Caps Lock = AUS, Num Lock = AN, Insert = AN (Einfüge-Modus)

| Taste | Weg vom Normal (→ Rot) | Zurück zum Normal (→ Grün) | Danach |
|---|---|---|---|
| **Caps Lock** | AN → 5× rot pulsierend | AUS → 5× grün pulsierend | Abnormal: LED aus · Normal: Basisfarbe |
| **Num Lock** | AUS → 5× rot pulsierend | AN → 5× grün pulsierend | Abnormal: LED aus · Normal: Basisfarbe |
| **Insert** | AUS (Überschreiben) → 5× rot | AN (Einfügen) → 5× grün | Abnormal: LED aus · Normal: Basisfarbe |

> Die Puls-Animation funktioniert auch bei ausgeschalteter Hintergrundbeleuchtung.
> Das Pulsieren erfolgt zwischen Farbe und Schwarz.

**Insert-Taste Langdruck:** Da der Insert-Zustand vom Betriebssystem nicht zurückgemeldet wird, trackt die Firmware den Zustand selbst (Standard: AN). Falls das Tracking aus dem Takt gerät, kann man die Insert-Taste **2 Sekunden lang gedrückt halten** um den Tracking-Wert auf „AN“ (Einfüge-Modus) zurückzusetzen. Ein langer Tastendruck sendet dabei kein Insert-Signal an den Computer.

### 🔦 FN-Layer Beleuchtung

Beim Gedrückthalten der FN-Taste werden nur die Tasten beleuchtet, die im FN-Layer eine Funktion haben:

| Hintergrundbeleuchtung | Verhalten bei FN gedrückt |
|---|---|
| **AN** | Nur FN-belegte Tasten leuchten in der Basisfarbe, alle anderen aus |
| **AUS** | FN-belegte Tasten leuchten grün, alle anderen bleiben aus |

Beim Loslassen der FN-Taste kehrt die Beleuchtung sofort zum Normalzustand zurück.

### 🔢 Num Lock Indikator
- Num Lock **AN** → Die Num-Lock-Taste leuchtet in der gewählten Basisfarbe
- Num Lock **AUS** → Die Num-Lock-Taste ist dunkel

## 🔄 Original-Firmware wiederherstellen

Auf der offiziellen Seite von Glorious findest du die originale Firmware sowie eine Installationsanleitung, mit der du deine Tastatur wieder in den Auslieferungszustand versetzen kannst:

👉 **[GMMK 3 Support – Glorious](https://www.gloriousgaming.com/en-eu/blogs/guides-resources)**

---

## ⌨️ Vollständige Tastenbelegung

### Layer 0 – Basis (QWERTZ DE/AT) – Windows

```
┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐             ┌───┐
│Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Scr│Pse│             │Mut│
└───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘             └───┘
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐ ┌───┬───┬───┐   ┌───┬───┬───┬───┐
│ ^ │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ ß │ ´ │ Bksp│ │Ins│Hom│PgU│   │Num│ / │ * │ - │
├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬───┤ ├───┼───┼───┤   ├───┼───┼───┼───┤
│ Tab │ Q │ W │ E │ R │ T │ Z │ U │ I │ O │ P │ Ü │ + │   │ │Del│End│PgD│   │ 7 │ 8 │ 9 │   │
├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐  │ └───┴───┴───┘   ├───┼───┼───┤ + │
│ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Ö │ Ä │ # │⏎ │                 │ 4 │ 5 │ 6 │   │
├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴──┤       ┌───┐     │───┼───┼───┼───┤
│LSft│ < │ Y │ X │ C │ V │ B │ N │ M │ , │ . │ - │  RShift  │     │ ↑ │     │ 1 │ 2 │ 3 │   │
├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤Ent│
│Ctrl│Win │Alt │         Space          │RAlt│ FN │App │RCtl│ │ ← │ ↓ │ → │ │   0   │ . │   │
└────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
```

### Layer 1 – FN (gehalten) – Windows

```
┌──────┐   ┌──────┬──────┬──────┬──────┐ ┌──────┬──────┬──────┬──────┐ ┌──────┬──────┬──────┬──────┐ ┌──────┬──────┬──────┐             ┌─────┐
│ BOOT │   │MyCmp │Brows │ Calc │MedSl │ │ Prev │  RW  │ Play │ Stop │ │ Mute │Vol + │Vol - │  —   │ │  —   │  —   │  —   │             │SdTog│
└──────┘   └──────┴──────┴──────┴──────┘ └──────┴──────┴──────┴──────┘ └──────┴──────┴──────┴──────┘ └──────┴──────┴──────┘             └─────┘
┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬───────┐ ┌──────┬──────┬──────┐ ┌──────┬──────┬──────┬──────┐
│  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │   —   │ │  —   │  —   │ Home │ │  —   │  —   │  —   │  —   │
├──────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬─────┤ ├──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┤
│   —    │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │     │ │  —   │  —   │ End  │ │  —   │  —   │  —   │      │
├────────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┐    │ └──────┴──────┴──────┘ ├──────┼──────┼──────┤  —   │
│    —    │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │ —  │                        │  —   │  —   │  —   │      │
├────┬────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴──────┤          ┌──────┐           ├──────┼──────┼──────┼──────┤
│ —  │  —  │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │     —      │          │Brit+ │           │  —   │  —   │  —   │      │
├────┼─────┼──────┼──────────────────────────────────────┬──┴──┬───┴──┬───┴──┬───┴─┬──────────┘   ┌──────┼──────┼──────┐    ├──────┴──────┼──────┤  —   │
│ —  │WnTog│  —   │                  —                   │ —   │  —   │  —   │  —  │              │PrevC │Brit- │NextC │    │     —       │  —   │      │
└────┴─────┴──────┴──────────────────────────────────────┴─────┴──────┴──────┴─────┘              └──────┴──────┴──────┘    └─────────────┴──────┴──────┘

Legende: — = transparent (Taste aus Layer 0 wird verwendet)
         Brit+/- = Helligkeit +/–  |  Prev/Next C = Basisfarbe wechseln
         BOOT = Bootloader-Modus   |  SdTog = Seiten-LEDs an/aus
         WnTog = Windows-Taste sperren/entsperren
```

---

> 🔀 **Layer 2 & 3 werden über den DIP-Switch auf der Rückseite der Tastatur aktiviert.**

### Layer 2 – Basis (QWERTZ DE/AT) – Mac OS

Identisch mit Layer 0 (Windows), **außer** der untersten Tastenreihe:

```
├────┼─────┼──────┼──────────────────────────────────────┬──────┬────────┬──────┬──────┤ ┌───┼───┼───┐ ├───────┼───┤   │
│Ctrl│ Opt │ Cmd  │                Space                 │ Cmd  │   FN   │ Opt  │ Ctrl │ │ ← │ ↓ │ → │ │   0   │ . │   │
└────┴─────┴──────┴──────────────────────────────────────┴──────┴────────┴──────┴──────┘ └───┴───┴───┘ └───────┴───┴───┘
```

Statt `Win/Alt/RAlt` kommen die Mac-Modifier `Opt` (Option/Alt) und `Cmd` (Command/⌘) zum Einsatz.

### Layer 3 – FN (gehalten) – Mac OS

Identisch mit Layer 1 (Windows-FN), **außer**:
- Kein `WnTog` (Windows-Taste sperren) – nicht relevant unter macOS
- Alle anderen FN-Funktionen (Helligkeit, Farbe, Media-Tasten, Seiten-LEDs) sind identisch

---

## 📋 FN-Tasten Kurzreferenz

| FN + | Funktion |
|---|---|
| `↑` | 🔆 Helligkeit + |
| `↓` | 🔅 Helligkeit – |
| `→` | ➡️ Nächste Basisfarbe |
| `←` | ⬅️ Vorherige Basisfarbe |
| `Encoder drücken` | 💡 Seiten-LEDs an/aus |
| `ESC` | 🔧 Bootloader-Modus (zum Flashen) |
| `PgUp` | ⬆️ Pos1 / Home |
| `PgDn` | ⬇️ Ende / End |
| `Win` | 🔒 Windows-Taste sperren/entsperren |
| `F1` | 💻 Dateimanager |
| `F2` | 🌐 Browser |
| `F3` | 🧮 Taschenrechner |
| `F4` | 🎵 Mediathek |
| `F5` | ⏮ Vorheriger Titel |
| `F6` | ⏪ Rewind |
| `F7` | ⏯ Play / Pause |
| `F8` | ⏹ Stop |
| `F9` | 🔇 Mute |
| `F10` | 🔊 Lautstärke + |
| `F11` | 🔉 Lautstärke – |

### Encoder (ohne FN)

| Aktion | Funktion |
|---|---|
| Drehen rechts | 🔊 Lautstärke + |
| Drehen links | 🔉 Lautstärke – |
| Drücken | 🔇 Mute |

---

## 🛠️ Selbst kompilieren

Die Keymap-Quelldateien liegen im Ordner [`keymap_p100/`](keymap_p100/).

> ⚠️ **Wichtiger Hinweis:** Der Fork von Lud00 (der die GMMK3 P100 Board-Definition enthält) ist **nicht direkt kompilierbar** – er enthält mehrere Fehler/fehlende Dateien. Die folgende Anleitung beschreibt den tatsächlich funktionierenden Weg.

### Schritt 1: QMK-Basis (GloriousThrall-Fork) klonen

Dieser Fork wird als Basis verwendet, weil die Toolchain und alle Abhängigkeiten hier korrekt konfiguriert sind:

```bash
git clone https://github.com/GloriousThrall/qmk_firmware.git
cd qmk_firmware
qmk setup --yes
```

### Schritt 2: GMMK3 P100 Board-Definition hinzufügen

Der GloriousThrall-Fork enthält keine GMMK3-Definition. Diese muss aus dem Lud00-Fork übernommen werden – **mit folgenden Korrekturen**:

```bash
mkdir -p keyboards/gmmk/gmmk3/p100/iso/keymaps/default
```

Folgende Dateien aus dem [Lud00-Repository](https://github.com/Lud00/gmmk/tree/gmmk3_p100_ansi_iso_wired/keyboards/gmmk/gmmk3/p100) kopieren/erstellen:

| Datei | Quelle |
|---|---|
| `keyboards/gmmk/gmmk3/p100/config.h` | Lud00-Repo (unverändert) |
| `keyboards/gmmk/gmmk3/p100/info.json` | Lud00-Repo (unverändert) |
| `keyboards/gmmk/gmmk3/p100/iso/info.json` | Lud00-Repo (unverändert) |
| `keyboards/gmmk/gmmk3/p100/iso/rules.mk` | Lud00-Repo (unverändert) |
| `keyboards/gmmk/gmmk3/p100/iso/iso.c` | Lud00-Repo **+ Korrekturen (siehe unten)** |
| `keyboards/gmmk/gmmk3/p100/halconf.h` | Lud00-Repo (unverändert) |
| `keyboards/gmmk/gmmk3/p100/mcuconf.h` | Lud00-Repo (unverändert) |

#### Korrekturen in `iso.c` (zwingend erforderlich!)

Die `iso.c` aus dem Lud00-Repo kompiliert **nicht ohne** diese Änderungen:

**1. `#include QMK_KEYBOARD_H` ganz oben einfügen** (vor allen anderen Includes):
```c
// iso.c – ganz oben, nach dem Lizenzblock:
#include QMK_KEYBOARD_H   // ← NEU: fehlt im Original, Typen sonst unbekannt
#include "spi_master.h"
```
Ohne diesen Include sind `aw20216s_led_t` und `led_config_t` unbekannt → Compile-Fehler.

**2. `#define __ NO_LED` an den Anfang verschieben** (war fälschlicherweise mitten in der Datei zwischen den Daten-Arrays):
```c
// Gleich nach den Includes:
#define __ NO_LED
```

### Schritt 3: Keymap kopieren

```bash
cp -r /pfad/zu/keymap_p100 keyboards/gmmk/gmmk3/p100/iso/keymaps/iso_de_custom
```

### Schritt 4: Kompilieren

```bash
qmk compile -kb gmmk/gmmk3/p100/iso -km iso_de_custom
```

> 💡 **Unter Windows** muss dies im **MSYS2 MinGW 64-bit Terminal** ausgeführt werden (nicht in der normalen PowerShell). QMK unter Windows wird über [QMK MSYS](https://msys.qmk.fm/) installiert.

Die kompilierte `.bin`-Datei findet sich anschließend im QMK-Stammverzeichnis.

### Bekannte Fehler & Lösungen

| Fehlermeldung | Ursache | Lösung |
|---|---|---|
| `unknown type name 'aw20216s_led_t'` | Fehlender `#include QMK_KEYBOARD_H` in `iso.c` | Wie in Schritt 2 beschrieben einfügen |
| `SPI driver activated but no SPI peripheral assigned` | Fehlende `halconf.h` / `mcuconf.h` | Aus Lud00-Repo kopieren (Schritt 2) |
| `Number of keymap layers exceeds DYNAMIC_KEYMAP_LAYER_COUNT` | Standardwert ist 2, aber 4 Layer werden benötigt | In `config.h`: `#define DYNAMIC_KEYMAP_LAYER_COUNT 4` |

---

## 🔗 Verwandte Projekte

| Projekt | Beschreibung |
|---|---|
| [GMMK2 ISO-DE Custom Firmware](https://github.com/matthili/my_GMMK2_ISO-DE_custom_firmware) | Die Original-Firmware, von der dieses Projekt portiert wurde (GMMK2 96% und 65%) |
| [Lud00/gmmk – GMMK3 P100 Branch](https://github.com/Lud00/gmmk/tree/gmmk3_p100_ansi_iso_wired/keyboards/gmmk/gmmk3/p100) | Basis-Repository mit Keyboard-Definition und LED-Mappings für die GMMK3 P100 |
| [QMK Firmware](https://github.com/qmk/qmk_firmware) | Das QMK-Framework |
| [QMK Toolbox](https://github.com/qmk/qmk_toolbox/releases) | Tool zum Flashen der Firmware |

---

## 📄 Lizenz

Diese Keymap-Dateien stehen unter der **GNU General Public License v2** (GPL v2), entsprechend der Lizenz des QMK-Firmware-Projekts.

Das vollständige Basis-Framework (QMK) ist unter [qmk/qmk_firmware](https://github.com/qmk/qmk_firmware) verfügbar.

> *Die kompilierten `.bin`-Dateien enthalten QMK-Framework-Code, der ebenfalls unter GPL v2 steht. Der vollständige Quellcode dieser Anpassung ist im Ordner `keymap_p100/` dieses Repositories enthalten.*

---

## 🙋 Feedback & Beiträge

Wenn du Fehler findest, andere Layouts brauchst oder Verbesserungsvorschläge hast, freue ich mich über Issues oder Pull Requests!
