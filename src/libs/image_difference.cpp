// (c) Copyright 2016 Josh Wright
#include "voronoi/voronoi.h"
#include "image_difference.h"

namespace image_utils {

    double avg_sq_dist_same_size(const image_RGB &image1, const image_RGB &image2) {
        assert_same_size(image1, image2);
        double diff = 0.0;
        for (size_t i = 0; i < image1.size(); ++i) {
            vec3 c1 = RGB_to_vec3(image1(i));
            vec3 c2 = RGB_to_vec3(image2(i));
            diff += c1.dist2(c2);
        }
        return diff / image1.size();
    }

    double avg_sq_dist_same_size(const voronoi &voronoi1, const image_RGB &base) {
        assert_same_size(voronoi1.grid, base);
        std::vector<RGB> colors = voronoi1.cell_average_colors(base);
        double diff = 0.0;
        for (size_t i = 0; i < voronoi1.grid.size(); ++i) {
            vec3 c1 = RGB_to_vec3(colors[voronoi1.grid(i).point_index]);
            vec3 c2 = RGB_to_vec3(base(i));
            diff += c1.dist2(c2);
        }
        return diff / voronoi1.grid.size();
    }

}
