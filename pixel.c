#include "pixel.h"
#include "geometric.h"

Vector Px_2Vec(Pixel px) {
    return (Vector){
        .x = px.r / 255.,
        .y = px.g / 255.,
        .z = px.b / 255.,
    };
}
