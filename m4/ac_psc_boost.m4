AC_DEFUN([AC_PSC_LIBBOOST], 
[AC_PSC_DEPENDENCY([Boost], [boost], [BOOST], [boost/msm/back/state_machine.hpp], [boost_serialization], [-lboost_serialization -lboost_program_options], [1.47 or later], [yes], [no], [yes])])
