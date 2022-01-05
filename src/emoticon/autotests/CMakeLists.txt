# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
# Convenience macro to add unit tests.
macro( emoticontest _source )
  set( _test ${_source} )
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} ${_name}.h)
  add_test(NAME ${_name} COMMAND ${_name} )
  ecm_mark_as_test(kpimtextedit-${_name})

  target_link_libraries( ${_name} Qt::Test Qt::Widgets KF5::Codecs KF5PimTextEdit)
endmacro()

emoticontest(emoticontexteditselectortest.cpp)
emoticontest(emoticonlistviewtest.cpp)
emoticontest(emoticonunicodeproxymodeltest.cpp)
