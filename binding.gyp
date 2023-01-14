{
  "targets": [
    {
      "target_name": "zic_ui",
      "cflags!": [
        "-fno-exceptions",
        '<!@(pkg-config sdl2 --cflags --libs)'
      ],
      "cflags_cc!": [ 
        "-fno-exceptions",
      ],
      "sources": [ "src/zic_node_ui.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        '<!@(pkg-config sdl2 --cflags-only-I | sed s/-I//g)',
      ],
      'libraries': [
          '-lSDL2',
          '-lSDL2_ttf'
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
