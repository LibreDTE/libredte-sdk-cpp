SDK de LibreDTE para C++
========================

SDK para realizar la integración con los servicios web de LibreDTE desde C++.

Este código está liberado bajo licencia [LGPL](http://www.gnu.org/licenses/lgpl-3.0.en.html).
O sea, puede ser utilizado tanto en software libre como en software privativo.

Dependencias:

	# apt-get install build-essential libcurl4-openssl-dev

Descargar json:

	$ wget -c https://github.com/nlohmann/json/releases/download/v2.1.0/json.hpp -O src/json.hpp

Compilar ejemplos:

	$ make

Ejecutar ejemplos (modificar variables según corresponda):

	$ ./ejemplos/001-generar_dte
	$ ./ejemplos/002-actualizar_estado

**Nota**: el SDK no se compila aparte, todo lo necesario está en el archivo
*src/LibreDTE.hpp* que se incluye directamente en el proyecto/ejemplo.
