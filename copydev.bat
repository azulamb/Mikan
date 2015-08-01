MD C:\lib\Mikan2015
MD C:\lib\Mikan2015\Dev
FOR /F "tokens=1,2" %%a IN (copydevlist.txt) DO ( COPY %%a %%b /Y )