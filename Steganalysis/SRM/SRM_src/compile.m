% This compile.m is used under Windows
clc;
fprintf('compiling SRM ... ');

%% 64 bit Windows and Matlab
mex -O -largeArrayDims -output SRM ...
    SRM_matlab.cpp SRMclass.cpp submodel.cpp s.cpp image.cpp exception.cpp config.cpp ...
    -I../include

fprintf('done\n');
copyfile('*.mex*','../matlab');
fprintf('Compiled SRM MEX file was copied to ../matlab folder.\n');
