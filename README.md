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

### 🇦🇹 Deutsches ISO-Layout (QWERTZ)
- Vollständige QWERTZ-Tastenbelegung für DE/AT
- Umlaute direkt auf den gewohnten Positionen: **Ö, Ä, Ü, ß**
- Sonderzeichen korrekt: **^, <, #, +, ´**
- Z und Y korrekt vertauscht gegenüber QWERTY

### 🎛️ VIA-Unterstützung
- VIA-Support ist aktiviert (`VIA_ENABLE = yes`)
- Tastenänderungen können jederzeit per Browser über **[usevia.app](https://usevia.app)** vorgenommen werden, ohne neu zu kompilieren
- 2 Keymap-Layer sind im EEPROM gespeichert

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

### 🔢 Num Lock Indikator
- Num Lock **AN** → Die Num-Lock-Taste leuchtet in der gewählten Basisfarbe
- Num Lock **AUS** → Die Num-Lock-Taste ist dunkel

---

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
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
│ ^ │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ ß │ ´ │ Bksp │ │Ins│Hom│PgU│ │Num│ / │ * │ - │
├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬───┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
│ Tab │ Q │ W │ E │ R │ T │ Z │ U │ I │ O │ P │ Ü │ + │   │ │Del│End│PgD│ │ 7 │ 8 │ 9 │   │
├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐  │ └───┴───┴───┘ ├───┼───┼───┤ + │
│ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Ö │ Ä │ # │⏎ │               │ 4 │ 5 │ 6 │   │
├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴──┤     ┌───┐     ├───┼───┼───┼───┤
│LSft│ < │ Y │ X │ C │ V │ B │ N │ M │ , │ . │ - │  RShift  │     │ ↑ │     │ 1 │ 2 │ 3 │   │
├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤Ent│
│Ctrl│Win │Alt │         Space           │RAlt│ FN │App │RCtl│ │ ← │ ↓ │ → │ │   0   │ . │   │
└────┴────┴────┴─────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
```

### Layer 1 – FN (gehalten) – Windows

```
┌──────┐   ┌──────┬──────┬──────┬──────┐ ┌──────┬──────┬──────┬──────┐ ┌──────┬──────┬──────┬──────┐ ┌──────┬──────┬──────┐             ┌─────┐
│ BOOT │   │MyCmp │Brows │ Calc │MedSl │ │ Prev │  RW  │ Play │ Stop │ │ Mute │Vol + │Vol - │  —   │ │  —   │  —   │  —   │             │SdTog│
└──────┘   └──────┴──────┴──────┴──────┘ └──────┴──────┴──────┴──────┘ └──────┴──────┴──────┴──────┘ └──────┴──────┴──────┘             └─────┘
┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬────────┐ ┌──────┬──────┬──────┐ ┌──────┬──────┬──────┬──────┐
│  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │   —   │ │  —   │  —   │ Home │ │  —   │  —   │  —   │  —   │
├──────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬────┴─┬─────┤ ├──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┤
│   —    │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │     │ │  —   │  —   │ End  │ │  —   │  —   │  —   │      │
├────────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┐    │ └──────┴──────┴──────┘ ├──────┼──────┼──────┤  —   │
│    —    │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │ —  │                         │  —   │  —   │  —   │      │
├────┬────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴┬─────┴──────┤          ┌──────┐            ├──────┼──────┼──────┼──────┤
│ —  │  —  │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │     —      │          │Brit+ │            │  —   │  —   │  —   │      │
├────┼─────┼──────┼──────────────────────────────────────┬────┴┬─────┴┬─────┴┬─────┤ ┌──────┼──────┼──────┐ ├──────┴──────┼──────┤  —   │
│ —  │WnTog│  —   │                  —                   │ —   │  —   │  —   │  —  │ │PrevC │Brit- │NextC │ │     —       │  —   │      │
└────┴─────┴──────┴──────────────────────────────────────┴─────┴──────┴──────┴─────┘ └──────┴──────┴──────┘ └─────────────┴──────┴──────┘

Legende: — = transparent (Taste aus Layer 0 wird verwendet)
         Brit+/- = Helligkeit +/–  |  Prev/Next C = Basisfarbe wechseln
         BOOT = Bootloader-Modus   |  SdTog = Seiten-LEDs an/aus
         WnTog = Windows-Taste sperren/entsperren
```

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

Als Basis-Repository wird der Fork von Lud00 benötigt, da nur dieser die Keyboard-Definition für die GMMK3 P100 enthält:

👉 **[Lud00/gmmk – Branch: gmmk3_p100_ansi_iso_wired](https://github.com/Lud00/gmmk/tree/gmmk3_p100_ansi_iso_wired/keyboards/gmmk/gmmk3/p100)**

**Kurzanleitung:**

```bash
# 1. Repository klonen (den richtigen Branch!)
git clone -b gmmk3_p100_ansi_iso_wired https://github.com/Lud00/gmmk.git qmk_firmware
cd qmk_firmware
git submodule update --init --recursive lib/lufa

# 2. Keymap-Ordner kopieren
cp -r /pfad/zu/keymap_p100 keyboards/gmmk/gmmk3/p100/iso/keymaps/iso_de_custom

# 3. Kompilieren
qmk compile -kb gmmk/gmmk3/p100/iso -km iso_de_custom
```

Die kompilierte Firmware-Datei findet sich anschließend im QMK-Stammverzeichnis.

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
