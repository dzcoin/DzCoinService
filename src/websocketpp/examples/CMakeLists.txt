﻿file (glob sdirs relative ${cmake_current_source_dir} *)
foreach (subdir ${sdirs})
    if (exists "${cmake_current_source_dir}/${subdir}/cmakelists.txt")
        add_subdirectory (${subdir})
    endif ()
endforeach ()


