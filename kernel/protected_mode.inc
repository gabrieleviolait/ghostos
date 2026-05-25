; Compatibility wrapper: protected-mode runtime now lives in kernel/protected/.

%include "kernel/protected/gdt.inc"
%include "kernel/protected/idt.inc"
%include "kernel/protected/irq.inc"
%include "kernel/protected/paging.inc"
%include "kernel/protected/memory.inc"
%include "kernel/protected/page_alloc.inc"
%include "kernel/protected/heap.inc"
%include "kernel/protected/tasks.inc"
%include "kernel/protected/keyboard.inc"
%include "kernel/protected/vga32.inc"
%include "kernel/protected/fat12.inc"
%include "kernel/protected/browser32.inc"
%include "kernel/protected/gex.inc"
%include "kernel/protected/shell32.inc"

[BITS 16]
