NOTA for the maintainer : this is the app's root README, for all the app,
bot the one dedicated to this sub-component.

*XDHDq* is an application which has been developed as proof of concept of the *XDHTML* technology.

*XDHDq* comes with different components. Each component has its own copy of the *Epeios* framework in `src/epeios` (not the entire framework; only the modules it needs).

Unlike other *Epeios* related applications, there is no `processing`
directory (which usually contains the processing tools and the backend),
because this application focuses on the *XDHTML* technology, which only deals with user interfaces.
Also, although this application has no backend, there are some backend-related files, such as 'frdapi.h'. This file acts only as placeholder and facilitates the coding. 

*XDHDq* home page :  http://q37.info/computing/epeios/apps/xdhdq/.