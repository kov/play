project('tictactoe', 'c')

gnome = import('gnome')
marshal_files = gnome.genmarshal('gtktictactoe-marshal',
    sources: 'gtktictactoe-marshal.list',
    prefix: 'gtk_tictactoe_marshal'
)

executable('tictactoe',
    marshal_files,
    'gtktictactoe.c',
    'main.c',
     dependencies: dependency('gtk+-3.0')
)
