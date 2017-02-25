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

/**
 * Ejemplo que muestra los pasos para:
 *  - Actualizar el estado de un DTE real
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-24
 */

#include "../src/LibreDTE.hpp"

int main()
{
	std::string url = "https://libredte.cl";
	std::string hash = "";
	int rut = 76192083;
	int dte = 33;
	int folio = 42;
	int metodo = 1; // =1 servicio web, =0 correo
	LibreDTE cliente = LibreDTE(hash, url);
	RestResponse estado;
	std::ostringstream recurso;
	recurso << "/dte/dte_emitidos/actualizar_estado/" << dte << "/" << folio << "/" << rut << "?usarWebservice=" << metodo;
	estado = cliente.get(recurso.str());
	if (estado.code != 200) {
		std::cerr << "Error al obtener el estado del DTE emitido: " << estado.body << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << estado.body << std::endl;
	return EXIT_SUCCESS;
}
