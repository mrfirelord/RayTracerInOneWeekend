#pragma once

#include <glm/glm.hpp>
#include <iostream>



void write_color(std::ostream& out, glm::vec3 color) {
	//Translate [0,1] to [0,255] and write it to output.
	out << static_cast<int>(255.999 * color.r) << ' '
		<< static_cast<int>(255.999 * color.g) << ' '
		<< static_cast<int>(255.999 * color.b) << '\n';
}
