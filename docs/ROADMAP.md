# Ghost OS roadmap

Versione corrente: v0.5.4

## Fase 0 - v0.5.2 stabile

- Checkpoint v0.5.2 allineato in bootloader, kernel e messaggi.
- Bootloader con `KERNEL_SECTORS = 32`.
- Kernel fissato a `KERNEL_SIZE = 16384` byte.
- Comandi base da verificare: `help`, `gui`, `browse`, `calc`, `color`, backspace.

## Fase 1 - modularizzazione minima

- `kernel/config.inc`: costanti di build e layout.
- `kernel/video_vga.inc`: modalita' testo, stampa, input riga, cursore.
- `kernel/string.inc`: parsing stringhe per la shell.
- `kernel/math.inc`: helper numerici.
- `kernel/browser.inc`: browser testuale integrato.
- `kernel/commands.inc`: implementazioni dei comandi.
- `kernel/shell.inc`: loop e dispatch della shell.
- `kernel/data.inc`: stato globale e stringhe.

## v0.5.3 - multi-digit calculator

- Fatto: `parse_uint16`.
- Fatto: `calc` con numeri multi-cifra.
- Fatto: operatori `+`, `-`, `*`, `/`.
- Fatto: errori per input non valido, overflow, risultato negativo e divisione per zero.

## v0.5.4 - scrolling terminal

- Fatto: `scroll_up` nel driver VGA.
- Fatto: righe 1-24 copiate in 0-23.
- Fatto: ultima riga pulita.
- Fatto: scroll quando `cursor_y >= 25`.
- Fatto: comando `stress` / `flood` per testare stampa lunga.

## v0.5.5 - windows/panels TUI

- `draw_box`.
- `draw_title`.
- Dashboard/pannelli testuali per comando `desktop` o `gui`.

## v0.6 - FAT12 filesystem

- Lettura root directory FAT12.
- Ricerca file 8.3.
- Caricamento file in buffer.
- Preparazione immagine floppy con file reali, non solo settori raw.

## v0.7 - load `.GHT` pages from disk

- Comando `browse HOME.GHT`.
- Parser minimale tag-like.
- Rendering testuale di pagine locali.

## v0.8 - protected mode

- GDT minimale.
- Salto 32-bit.
- Demo VGA protected mode senza BIOS.
