{
  "variables":
   {
    "epeios": "../../../stable/",
   }
  ,
  "targets": [
    {
      "target_name": "xppqnjs",
      "sources": [ "xppqnjs.cpp", "common.cpp", "stream.cpp", "parser.cpp", "<(epeios)/bso.cpp", "<(epeios)/cio.cpp", "<(epeios)/err.cpp", "<(epeios)/fdr.cpp", "<(epeios)/flw.cpp", "<(epeios)/tol.cpp", "<(epeios)/uys.cpp" , "<(epeios)/ntvstr.cpp", "<(epeios)/str.cpp", "<(epeios)/mtx.cpp", "<(epeios)/tht.cpp", "<(epeios)/dtfptb.cpp", "<(epeios)/ags.cpp", "<(epeios)/txf.cpp" , "<(epeios)/scln.cpp", "<(epeios)/v8q.cpp", "<(epeios)/v8qnjs.cpp", "<(epeios)/mtk.cpp", "<(epeios)/bomhdl.cpp", "<(epeios)/stsfsm.cpp", "<(epeios)/xpp.cpp", "<(epeios)/fnm.cpp", "<(epeios)/fil.cpp", "<(epeios)/ias.cpp", "<(epeios)/xml.cpp", "<(epeios)/tagsbs.cpp", "<(epeios)/lcl.cpp", "<(epeios)/cdgb64.cpp", "<(epeios)/rgstry.cpp", "<(epeios)/flx.cpp", "<(epeios)/uvq.cpp" ],
	  "include_dirs":  [ "<(epeios)" ],
	  "defines": [ "E_DEBUG" ],
	   'msvs_settings': {
                  'VCCLCompilerTool': {
                    'ExceptionHandling': 1
                  }
		},
		"cflags_cc": ["-std=gnu++11", "-fexceptions"],
    }
  ]
}