# Ghost OS roadmap

Versione corrente: v1.3-alpha

## Fase 0 - v0.5.2 stabile

- Checkpoint v0.5.2 allineato in bootloader, kernel e messaggi.
- Legacy v0.5.x: bootloader con `KERNEL_SECTORS = 32`.
- Legacy v0.5.x: kernel fissato a `KERNEL_SIZE = 16384` byte.
- Da v1.2-alpha: kernel esteso a 64 settori / 32768 byte per paging, IDT e bitmap frame.
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

- Fatto: immagine floppy FAT12 con kernel in settori riservati.
- Fatto: `Makefile` usa `tools/mkfat12.py` per creare image, boot sector, kernel e `.GHT`.
- Fatto: bootloader ancora carica kernel da settori fissi.
- Fatto: `kernel/fat12.inc`.
- Fatto: lettura boot sector FAT12, root directory, ricerca 8.3, cluster chain, file buffer.
- Fatto: `pages/HOME.GHT`, `pages/ABOUT.GHT`, `pages/HELP.GHT`.
- Fatto: comandi `ls` e `cat HOME.GHT`.

## v0.7 - load `.GHT` pages from disk

- Fatto: `browser.inc` legge argomento dopo `browse`.
- Fatto: se vuoto apre `HOME.GHT`.
- Fatto: chiama `fat12_read_file`.
- Fatto: carica file in `page_buffer`.
- Fatto: renderer `.GHT` minimale.
- Fatto: pseudo-tag semplici: `<title>`, `<h1>`, `<p>`, `<hr>`, `<pre>`, `<a href="">`.
- Fatto: link per ora solo visivi.
- Fatto: comandi `browse`, `browse HOME.GHT`, `browse ABOUT.GHT`.

## v0.8 - protected mode

- Fatto: `kernel/protected_mode.inc`.
- Fatto: GDT minimale.
- Fatto: disabilita interrupt, carica GDTR, imposta PE in CR0.
- Fatto: far jump in codice 32-bit.
- Fatto: stampa `Protected mode entered` direttamente in VGA memory.
- Fatto: halt.
- Nota: demo sicura e one-way; la shell resta 16-bit per ora.

## v0.9 - protected mode shell

- Fatto: ingresso stabile in protected mode all'avvio del kernel.
- Fatto: GDT pulita con code/data flat 32-bit.
- Fatto: IDT minimale.
- Fatto: remap PIC 8259.
- Fatto: IRQ tastiera abilitato.
- Fatto: lettura scancode da porta `0x60`.
- Fatto: conversione scancode ASCII base.
- Fatto: keyboard buffer.
- Fatto: `print_char` su VGA `0xB8000` 32-bit.
- Fatto: `shell_loop` 32-bit.
- Fatto: comandi minimi `help`, `clear`, `echo`, `version`, `halt`.

## v1.0-beta - stabilization + protected runtime cleanup

- Fatto: memory map BIOS E820 raccolta prima del protected mode.
- Fatto: mappa E820 salvata in memoria kernel.
- Fatto: bitmap fisica dei frame da 4 KB.
- Fatto: `alloc_frame` / `free_frame`.
- Fatto: paging base.
- Fatto: identity map dei primi 4 MB, incluso primo MB e kernel.
- Fatto: heap semplice.
- Fatto: `kmalloc` / `kfree` minimale.
- Fatto: struct Task concettuale.
- Fatto: stati `READY`, `RUNNING`, `BLOCKED`.
- Fatto: scheduler cooperativo iniziale.
- Fatto: `yield`.
- Fatto: Task 1 shell.
- Fatto: Task 2 demo counter.
- Fatto: runtime protected mode diviso in `kernel/protected/*.inc`.
- Fatto: panic screen per eccezioni fatali in protected mode.
- Fatto: `ghost32>` legge FAT12 read-only con `ls`, `cat` e `browse`.
- Fatto: driver floppy protected-mode minimale via FDC/DMA, senza BIOS `int 13h`.

## v1.2-alpha - protected memory runtime

- Fatto: errori FAT12 in `ghost32>` distinti tra nome invalido, file mancante, disco e buffer troppo piccolo.
- Fatto: page allocator con conteggi total/managed/free/used.
- Fatto: heap bump allocator con header, statistiche e `kfree` LIFO-safe.
- Fatto: comandi `heapinfo`, `pages`, `memmap`.
- Design-only: formato `.GEX` documentato per v1.3, senza loader runtime.

## v1.3-alpha - first .GEX loader

- Fatto: comando protected-mode `run FILE`.
- Fatto: validazione header `.GEX` con magic `GEX1`, entry, load address, size e flags.
- Fatto: loader ring 0 minimale con load fisso a `0x00180000`.
- Fatto: syscall table minimale `print`, `read`, `exit`.
- Fatto: sample `HELLO.GEX` buildato da NASM e copiato nella FAT12 image.
