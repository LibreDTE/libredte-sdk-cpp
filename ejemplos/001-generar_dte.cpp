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
 *  - Emitir DTE temporal
 *  - Generar DTE real a partir del temporal
 *  - Obtener PDF a partir del DTE real
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-25
 */

#include "../src/LibreDTE.hpp"
#include "../src/json.hpp"
using json = nlohmann::json;

std::string dte_crear()
{
	json dte = {
		{"Encabezado", {
			{"IdDoc", {
				{"TipoDTE", 33}
			}},
			{"Emisor", {
				{"RUTEmisor", "76192083-9"}
			}},
			{"Receptor", {
				{"RUTRecep", "66666666-6"},
				{"RznSocRecep", "Persona sin RUT"},
				{"GiroRecep", "Particular"},
				{"DirRecep", "Santiago"},
				{"CmnaRecep", "Santiago"}
			}}
		}},
		{"Detalle", {
			{
				{"NmbItem", "Producto 1"},
				{"QtyItem", 2},
				{"PrcItem", 1000}
			}
		}}
	};
	return dte.dump();
}

void dte_guardar_pdf(const char *file, const char *data, size_t size)
{
	size_t i;
	FILE *fd = fopen(file, "wb");
	if (!fd) {
		fprintf(stderr, "Error al guardar el PDF\n");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<size; i++) {
		fwrite(data+i, sizeof(char), 1, fd);
	}
	fclose(fd);
}

int main()
{
	std::string url = "https://libredte.cl";
	std::string hash = "";
	LibreDTE cliente = LibreDTE(hash, url);
	RestResponse emitir, generar, generar_pdf;
	std::ostringstream recurso;
	// crear dte temporal
	emitir = cliente.post("/dte/documentos/emitir", dte_crear());
	if (emitir.code != 200) {
		std::cerr << "Error al emitir DTE temporal: " << emitir.body << std::endl;;
		return EXIT_FAILURE;
	}
	// crear dte real
	generar = cliente.post("/dte/documentos/generar", emitir.body);
	if (generar.code != 200) {
		std::cerr << "Error al generar DTE real: " << generar.body << std::endl;
		return EXIT_FAILURE;
	}
	// obtener pdf
	json dte_emitido = json::parse(generar.body);
	recurso << "/dte/dte_emitidos/pdf/" << dte_emitido["dte"] << "/" << dte_emitido["folio"] << "/" << dte_emitido["emisor"];
	generar_pdf = cliente.get(recurso.str());
	if (generar_pdf.code != 200) {
		std::cerr << "Error al generar DTE real: " << generar_pdf.body;
		return EXIT_FAILURE;
	}
	dte_guardar_pdf("documento.pdf", generar_pdf.body.c_str(), generar_pdf.body.size());
	return EXIT_SUCCESS;
}
