#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  hit_record current_rec;
  bool has_hit = false;
  auto closest = t_max;

  for (const auto& object : objects) {
    if (object->hit(r, t_min, closest, current_rec)) {
      has_hit = true;
      closest = current_rec.t;
      rec = current_rec;
    }
  }

  return has_hit;
}

#endif