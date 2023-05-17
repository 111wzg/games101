//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

// 在场景的所有光源上按面积 uniform 地 sample 一个点，并计算该 sample 的概率密度
void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Intersection scene_pos = intersect(ray);
    if(!scene_pos.happened){
        return Vector3f(0, 0, 0);
    }
    if(scene_pos.m->hasEmission()){
        return scene_pos.m->getEmission();
    }


    Intersection light_pos;
    float pdf;
    sampleLight(light_pos, pdf);
    Vector3f p = scene_pos.coords;
    Vector3f x = light_pos.coords;
    Vector3f ws = (x - p).normalized();
    float d = std::pow((x - p).norm(), 2);
    Vector3f NN = light_pos.normal.normalized();
    Vector3f emit = light_pos.emit;
    Vector3f N = scene_pos.normal.normalized();
    Material* m = scene_pos.m;
    Ray px(p, ws);

    Vector3f L_dir;
    Intersection ray_middle = intersect(px);
    if((ray_middle.coords - x).norm() < 0.001){
        L_dir = emit * m->eval(ray.direction, ws, N) * dotProduct(ws, N) * dotProduct(-ws, NN) / d / pdf;
    }

    Vector3f L_indir;
    float ksi = get_random_float();
    if(ksi > RussianRoulette){
        return L_dir;
    }
    Vector3f wi = m->sample(ray.direction, N).normalized();
    Ray r(p, wi);
    Intersection r_pos = intersect(r);
    //我操，居然是if里两个条件不能换顺寻
    if(r_pos.happened && !r_pos.m->hasEmission()){
        L_indir = castRay(r, depth+1) * m->eval(ray.direction, wi, N) * dotProduct(wi, N) / m->pdf(ray.direction, wi, N) / RussianRoulette;
    }

    return L_indir + L_dir;
}