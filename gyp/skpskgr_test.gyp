# GYP file to build unit tests.
{
  'includes': [
    'apptype_console.gypi',
  ],
  'targets': [
    {
      'target_name': 'skpskgr_test',
      'type': 'executable',
      'suppress_wildcard': '1',
      'include_dirs' : [
        '../src/core',
        '../src/effects',
        '../src/lazy',
        '../src/pathops',
        '../src/pdf',
        '../src/pipe/utils',
        '../src/utils',
        '../tools/',
      ],
      'sources': [
        '../tests/SkpSkGrTest.cpp',
        '../tests/Test.cpp',
        '../tests/skia_test.cpp',
        '../tests/Test.h',
      ],
      'dependencies': [
        'skia_lib.gyp:skia_lib',
        'flags.gyp:flags',
      ],
      'conditions': [
        [ 'skia_gpu == 1', {
          'include_dirs': [
            '../src/gpu',
          ],
        }],
      ],
    },
  ],
}
