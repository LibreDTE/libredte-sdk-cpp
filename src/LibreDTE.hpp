#ifndef __LIBREDTE_HPP
#define __LIBREDTE_HPP

/**
 * LibreDTE
 * Copyright (C) SASCO SpA (https://sasco.cl)
 *
 * Este programa es software libre: usted puede redistribuirlo y/o modificarlo
 * bajo los términos de la GNU Lesser General Public License (LGPL) publicada
 * por la Fundación para el Software Libre, ya sea la versión 3 de la Licencia,
 * o (a su elección) cualquier versión posterior de la misma.
 *
 * Este programa se distribuye con la esperanza de que sea útil, pero SIN
 * GARANTÍA ALGUNA; ni siquiera la garantía implícita MERCANTIL o de APTITUD
 * PARA UN PROPÓSITO DETERMINADO. Consulte los detalles de la GNU Lesser General
 * Public License (LGPL) para obtener una información más detallada.
 *
 * Debería haber recibido una copia de la GNU Lesser General Public License
 * (LGPL) junto a este programa. En caso contrario, consulte
 * <http://www.gnu.org/licenses/lgpl.html>.
 */

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <curl/curl.h>

/**
 * Clase para respuestas de solicitudes Rest
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-24
 */
class RestResponse {
	public:
		int code;
		std::string body;
};

size_t rest_response_write(void *ptr, size_t size, size_t nmemb, void *body);

/**
 * Clase LibreDTE
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-24
 */
class LibreDTE {
	private:
		std::string url;
		std::string hash;
		std::string auth;
	public:
		LibreDTE(std::string, std::string);
		RestResponse post(std::string api, std::string data);
		RestResponse get(std::string api);
};

/**
 * Constructor de la clase LibreDTE
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-24
 */
LibreDTE::LibreDTE(std::string h, std::string u) {
	hash = h;
	url = u;
	std::ostringstream authStream;
	authStream << hash << ":X";
	auth = authStream.str();
}

/**
 * Método que realiza una consulta por POST
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-24
 */
RestResponse LibreDTE::post(std::string api, std::string data) {
	RestResponse response = RestResponse();
	std::ostringstream recurso;
	recurso << url << "/api" << api;
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	response.code = -1;
	if (!curl) {
		std::cerr << "curl_easy_init() falló!" << std::endl;
		exit(EXIT_FAILURE);
	}
	curl_easy_setopt(curl, CURLOPT_URL, recurso.str().c_str());
	curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());
	if (!data.empty()) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	}
	if (recurso.str()[4]=='s') {
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rest_response_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "curl_easy_perform() falló! " << curl_easy_strerror(res) << std::endl;
		std::cout << "  URL: " << recurso.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.code);
	curl_easy_cleanup(curl);
	return response;
}

/**
 * Método que realiza una consulta por GET
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-24
 */
RestResponse LibreDTE::get(std::string api) {
	return post(api, "");
}

/**
 * Función que escribe los datos de la respuesta del servicio web en la
 * estructura que corresponde a la respuesta del servicio
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
size_t rest_response_write(void *ptr, size_t size, size_t nmemb, void *body)
{
	((std::string*)body)->append((char*)ptr, size * nmemb);
	return size*nmemb;
}

#endif /* __LIBREDTE_HPP */
