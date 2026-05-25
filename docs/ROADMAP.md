# Ghost OS roadmap

Versione corrente: v0.5.5

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

- Fatto: `kernel/tui.inc`.
- Fatto: `draw_char_at`, `print_at`, `draw_hline`, `draw_vline`.
- Fatto: `draw_box`, `draw_title`, `clear_region`.
- Fatto: layout con header, status panel, commands panel, browser panel, footer.
- Fatto: `desktop` alias di `gui`.

## v0.6 - FAT12 filesystem

- Cambiare immagine floppy da raw settori a FAT12 formattata.
- Aggiornare `Makefile`: creare `ghostos.img`, formattare FAT12, scrivere bootloader nel primo settore, copiare kernel e file `.GHT`.
- Bootloader: per ora puo' ancora caricare kernel da settori fissi; poi possibile caricamento di `KERNEL.BIN` da FAT12.
- Creare `kernel/fat12.inc`.
- Kernel: leggere boot sector FAT12.
- Kernel: leggere root directory.
- Kernel: trovare file 8.3.
- Kernel: leggere cluster chain.
- Kernel: caricare file in buffer.
- Aggiungere `pages/HOME.GHT`, `pages/ABOUT.GHT`, `pages/HELP.GHT`.
- Comandi nuovi: `ls`, `cat HOME.GHT`.

## v0.7 - load `.GHT` pages from disk

- `browser.inc` legge argomento dopo `browse`.
- Se vuoto apre `HOME.GHT`.
- Chiama `fat12_read_file`.
- Carica file in `page_buffer`.
- Renderizza testo.
- Supporta pseudo-tag semplici: `<title>`, `<h1>`, `<p>`, `<hr>`, `<pre>`, `<a href="">`.
- Link inizialmente solo visivi.
- Poi supporto numeri link `[1]`, `[2]`.
- Comandi: `browse`, `browse HOME.GHT`, `browse ABOUT.GHT`.

## v0.8 - protected mode

- Creare `kernel/protected_mode.inc`.
- Definire GDT.
- Disabilitare interrupt.
- Caricare GDTR.
- Impostare bit PE in CR0.
- Far jump far in codice 32-bit.
- Stampare messaggio in VGA memory direttamente.
- Halt.
- Nota: demo sicura solo con messaggio `Protected mode entered`; la shell resta 16-bit per ora.
