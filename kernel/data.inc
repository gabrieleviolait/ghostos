; Kernel state, command names, and built-in text resources.

text_attr db 0x07

cursor_x db TEXT_LEFT_MARGIN
cursor_y db 0
print_char_value db 0

cmd_buffer times CMD_BUFFER_SIZE db 0

cmd_help    db 'help',0
cmd_clear   db 'clear',0
cmd_cls     db 'cls',0
cmd_echo    db 'echo',0
cmd_about   db 'about',0
cmd_version db 'version',0
cmd_gui     db 'gui',0
cmd_reboot  db 'reboot',0
cmd_halt    db 'halt',0
cmd_mem     db 'mem',0
cmd_ascii   db 'ascii',0
cmd_color   db 'color',0
cmd_calc    db 'calc',0
cmd_browse  db 'browse',0

banner db '================================================',13
       db '  Ghost OS v0.5.3',13
       db '  Minimal 16-bit Assembly OS / BIOS only',13
       db '================================================',13
       db '  Type help for commands. Type gui for dashboard.',13,13,0

prompt db 'ghost> ',0

help_msg db 'Commands:',13
         db '  help           show this help',13
         db '  clear / cls    clear screen',13
         db '  gui            show text dashboard',13
         db '  echo TEXT      print text',13
         db '  about          system info',13
         db '  version        show version',13
         db '  calc 12*34     multi-digit calculator',13
         db '  mem            show conventional memory KB',13
         db '  ascii          show internal ASCII logo',13
         db '  color N        set text color 0-F',13
         db '  reboot         BIOS reboot',13
         db '  halt           stop CPU',13,0

gui_msg db '+------------------------------------------------+',13
        db '|                  GHOST OS GUI                  |',13
        db '+------------------------------------------------+',13
        db '| Mode      : 16-bit real mode                   |',13
        db '| Kernel    : NASM Assembly                      |',13
        db '| Video     : BIOS text mode 80x25               |',13
        db '| Shell     : internal CLI                       |',13
        db '| Filesystem: not yet implemented                |',13
        db '+------------------------------------------------+',13
        db '| Commands: help  cls  echo  calc  mem  color    |',13
        db '|           about version ascii reboot halt      |',13
        db '+------------------------------------------------+',13,13,0

browser_page db '+------------------------------------------------+',13
             db '|                GHOST BROWSER v0.1              |',13
             db '+------------------------------------------------+',13
             db '| URL: ghost://home                              |',13
             db '+------------------------------------------------+',13
             db 13
             db '# Welcome to GhostNet',13
             db 13
             db 'This is the first local text-mode page rendered',13
             db 'inside Ghost OS.',13
             db 13
             db 'Supported pseudo-tags:',13
             db '  <title>  <h1>  <p>  <a>  <pre>  <hr>',13
             db 13
             db 'Links:',13
             db '  [1] ghost://about',13
             db '  [2] ghost://help',13
             db 13
             db 'Real network/Tor support will come later, after',13
             db 'filesystem, protected mode and network stack.',13
             db 13
             db '+------------------------------------------------+',13,13,0

about_msg db 'Ghost OS v0.5.3',13
          db 'Tiny educational OS written from scratch in NASM.',13
          db 'No Linux, no DOS, no external kernel. BIOS only.',13
          db 'Goal: small CLI-first assembly operating system.',13,0

version_msg db 'Ghost OS v0.5.3 / x86 16-bit real mode / NASM',13,0

unknown_msg db 'Unknown command. Type help.',13,0
reboot_msg  db 'Rebooting...',13,0
halt_msg    db 'System halted.',13,0
mem_msg     db 'Conventional memory: ',0
kb_msg      db ' KB',13,0
result_msg  db 'Result: ',0

negative_msg db 'Result is negative. v0.5.3 displays unsigned results only.',13,0
div_zero_msg db 'Division by zero.',13,0
overflow_msg db 'Result is too large for uint16.',13,0
calc_usage  db 'Usage: calc 12+34, calc 12-5, calc 7*8, or calc 20/4.',13,0
color_usage db 'Usage: color N where N is 0-F.',13,0
color_ok    db 'Color changed.',13,0

ascii_msg db '          __',13
          db '         / _)  Ghost OS',13
          db '  .-^^^-/ /',13
          db ' __/       /',13
          db '<__.|_|-|_|',13,0
