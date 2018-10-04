del _bundle\evalcom2.zip
del _bundle\evalcom2_srv.zip

copy MANIFEST.XML.client MANIFEST.XML
"C:\Program Files\WinRAR\winrar.exe" a -ep _bundle\evalcom2.zip MANIFEST.XML _output\Release\evalcom2.dll _output\Release_x64\evalcom2_x64.dll
del MANIFEST.xml

copy MANIFEST.XML.server MANIFEST.XML
"C:\Program Files\WinRAR\winrar.exe" a -ep _bundle\evalcom2_srv.zip MANIFEST.XML _output\Release\evalcom2.dll _output\Release_x64\evalcom2_x64.dll
del MANIFEST.XML