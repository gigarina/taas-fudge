# adm-fudge v1.0
adm-fudge is based on taas-fudge developed by:
Matthias Thimm (matthias.thimm@fernuni-hagen.de)
Federico Cerutti (federico.cerutti@unibs.it)
Mauro Vallati (m.vallati@hud.ac.uk)

All changes were performed by:
Carina Benzin (carinabenzin@outlook.com)

adm-fudge is a c++ program. 
To compile it you need the glib 2.0 header files (https://developer.gnome.org/glib/).

You can compile adm-fudge with
```
    ./build-adm-fudge.sh
```

adm-fudge uses taas-fudges probo-compliant command line interface, see also
http://argumentationcompetition.org/2021/SolverRequirements.pdf. 

adm-fudge implements [DC-CO, DC-PR] by deciding credulous acceptance regarding admissible extensions.
The solver only accepts .tgf files.

An example of a valid input for adm-fudge is the following:
```
  ./adm-fudge -p DC-CO -fo tgf -f <TGF-FILE> -a <ARGUMENT>
```
