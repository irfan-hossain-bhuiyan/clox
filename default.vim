let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/projects/clox
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +38 src/vm.c
badd +11 header/chunk.h
badd +190 src/debug.c
badd +46 src/compiler.c
badd +28 src/value.c
badd +24 ~/projects/clox/header/value.h
badd +108 main.c
badd +10 header/vm.h
badd +209 test/tests.c
badd +38 src/logging.c
badd +0 ~/projects/clox/include/logging.h
badd +5 ~/projects/clox/header/logging.h
badd +0 ~/projects/clox/header/debug.h
badd +35 CMakeLists.txt
badd +0 temp.c
badd +0 temp2.c
argglobal
%argdel
set stal=2
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit src/debug.c
argglobal
balt src/value.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
117
normal! zo
124
normal! zo
126
normal! zo
135
normal! zo
136
normal! zo
138
normal! zo
140
normal! zo
142
normal! zo
144
normal! zo
146
normal! zo
148
normal! zo
150
normal! zo
152
normal! zo
154
normal! zo
156
normal! zo
158
normal! zo
160
normal! zo
162
normal! zo
168
normal! zo
166
normal! zo
167
normal! zo
168
normal! zo
170
normal! zo
172
normal! zo
174
normal! zo
176
normal! zo
178
normal! zo
180
normal! zo
198
normal! zo
198
normal! zo
let s:l = 190 - ((27 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 190
normal! 03|
tabnext
edit CMakeLists.txt
argglobal
balt src/debug.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 35 - ((29 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 35
normal! 038|
tabnext
edit src/compiler.c
argglobal
balt test/tests.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
170
normal! zo
174
normal! zo
175
normal! zo
178
normal! zo
181
normal! zo
184
normal! zo
187
normal! zo
190
normal! zo
193
normal! zo
196
normal! zo
199
normal! zo
202
normal! zo
208
normal! zo
229
normal! zo
let s:l = 207 - ((25 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 207
normal! 0
tabnext
edit test/tests.c
argglobal
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 209 - ((15 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 209
normal! 018|
tabnext
edit temp2.c
argglobal
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 80 - ((17 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 80
normal! 05|
tabnext
edit temp.c
argglobal
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 1 - ((0 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
edit ~/projects/clox/header/debug.h
argglobal
balt src/debug.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
1
normal! zo
let s:l = 1 - ((0 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
edit header/chunk.h
argglobal
balt src/compiler.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
1
normal! zo
6
normal! zo
25
normal! zo
let s:l = 16 - ((15 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! 018|
tabnext
edit src/vm.c
argglobal
balt src/compiler.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
15
normal! zo
21
normal! zo
35
normal! zo
40
normal! zo
42
normal! zo
43
normal! zo
46
normal! zo
47
normal! zo
49
normal! zo
51
normal! zo
55
normal! zo
58
normal! zo
69
normal! zo
70
normal! zo
74
normal! zo
84
normal! zo
85
normal! zo
90
normal! zo
91
normal! zo
98
normal! zo
102
normal! zo
106
normal! zo
110
normal! zo
123
normal! zo
126
normal! zo
129
normal! zo
152
normal! zo
155
normal! zo
let s:l = 42 - ((12 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 42
normal! 0
tabnext
edit header/vm.h
argglobal
balt src/vm.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
1
normal! zo
6
normal! zo
11
normal! zo
let s:l = 24 - ((23 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 24
normal! 026|
tabnext
edit src/compiler.c
argglobal
balt src/vm.c
setlocal fdm=expr
setlocal fde=nvim_treesitter#foldexpr()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
34
normal! zo
40
normal! zo
42
normal! zo
63
normal! zo
65
normal! zo
67
normal! zo
128
normal! zo
151
normal! zo
158
normal! zo
159
normal! zo
165
normal! zo
170
normal! zo
174
normal! zo
175
normal! zo
178
normal! zo
181
normal! zo
184
normal! zo
193
normal! zo
196
normal! zo
199
normal! zo
202
normal! zo
208
normal! zo
229
normal! zo
let s:l = 193 - ((12 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 193
normal! 02|
tabnext 2
set stal=1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
