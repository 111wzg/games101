#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

//        Comment-in this part when you implement the constructor
//        for (auto &i : pinned_nodes) {
//            masses[i]->pinned = true;
//        }
        // float xnor = (end.x - start.x) / (num_nodes - 1);
        // float ynor = (end.y - start.y) / (num_nodes - 1);
        Vector2D nor = (end - start) / (num_nodes - 1); 
        for(int i=0; i<num_nodes; ++i){
            // Vector2D position = Vector2D(start.x + i * xnor, start.y + i * ynor);
            Vector2D position = start + i * nor;
            masses[i] = Mass(position, node_mass, false);
        }
        for (auto &i : pinned_nodes) {
            masses[i]->pinned = true;
        }
        for(int i=0; i<num_nodes-1; ++i){
            springs[i] = Spring(masses[i], masses[i+1], k);
        }

    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a nodes
            Vector2D b_a = (s->m2->position - s->m1->position).unit();
            double b_anor = (s->m2->position - s->m1->position).norm();
            s->m1->forces += s->k * b_a * (b_anor - s->rest_length);
            s->m2->forces += -s->m1->forces;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                
                // TODO (Part 2): Add global damping
                Vector2D a = m->forces / m->mass + gravity;
                //显式
                m->position += m->velocity * delta_t;
                m->velocity += a * delta_t;
                //半隐式
                // m->velocity += a * delta_t;
                // m->position += m->velocity * delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D b_a = (s->m2->position - s->m1->position).unit();
            double b_anor = (s->m2->position - s->m1->position).norm();
            s->m1->forces += s->k * b_a * (b_anor - s->rest_length);
            s->m2->forces += -s->m1->forces;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                
                // TODO (Part 4): Add global Verlet damping
                Vector2D a = m->forces / m->mass + gravity;
                Vector2D xt = m->position;
                float damping_factor = 0.00005;
                m->position = xt + (1 - damping_factor) * (xt - m->last_position) + a * delta_t * delta_t;
            }
            //消除所有力
            m->forces = Vector2D(0, 0);
        }
    }
}
