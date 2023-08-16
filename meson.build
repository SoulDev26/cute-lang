project('cute-lang', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

m_dep = cc.find_library('m')

cutevm_src = [
  'src/murmur3_32_hash.c',

  'src/vm/vm.c',
  'src/vm/object.c',
  'src/vm/ops.c',
  'src/vm/sys_libs.c',

  'src/vm/types/string.c',
  'src/vm/types/vector.c',
  'src/vm/types/hashmap.c',

  'src/vm/vm_types/fn.c',
  'src/vm/vm_types/numeric.c',
  'src/vm/vm_types/string.c',
  'src/vm/vm_types/vector.c',
  'src/vm/vm_types/bool.c',
  'src/vm/vm_types/unknown.c',
]

cutevm_std_src = [
  'src/vm_std/std.c',

  'src/vm_std/math.c',
  'src/vm_std/nums.c',
  'src/vm_std/str.c',
  'src/vm_std/io.c',
]

if get_option('buildtype') == 'debug'
  add_project_arguments('-DCUTEVM_DEBUG', language : 'c')
endif

#add_project_arguments('-Wl, --allow-shlib-undefined', language : 'c')
#add_project_arguments('-Wl, --unresolved-symbols=ignore-in-object-files', language : 'c')
add_project_arguments('-Wno-pedantic', language : 'c')

executable('cutevm', 'cutevm.c', cutevm_src,
  dependencies : [m_dep],
  link_args : ['-Wl,--export-dynamic'])

shared_library('std', cutevm_std_src,
  link_args: ['-Wl,--unresolved-symbols=ignore-in-object-files'])
