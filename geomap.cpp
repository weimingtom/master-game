#include "geomap.hpp"

void mapdraw(VG_Node *vgroot )
{
        VG_Line* Line1;
    VG_Vector vect1,vect2,vect3;
    vect1.x=200;
    vect1.y=200;
    vect2.x=400;
    vect2.y=200;
    vect3.x=200;
    vect3.y=400;


    //void VG_NodeInit (VG_Node *node, VG_NodeOps *class)
    VG_Point* VP1=VG_PointNew(vgroot, vect1);
    VG_Point* VP2=VG_PointNew(vgroot, vect2);
    VG_Point* VP3=VG_PointNew(vgroot, vect3);

    //VisiLibity::Point ptv1;
    VisiLibity::Point ptv1 = VisiLibity::Point(200,200);
    VisiLibity::Point ptv2 = VisiLibity::Point(400,200);
    VisiLibity::Point ptv3 = VisiLibity::Point(200,400);

    std::vector < VisiLibity::Point > vertices_temp;

    vertices_temp.push_back(ptv1);
    vertices_temp.push_back(ptv2);
    vertices_temp.push_back(ptv3);

    VisiLibity::Polygon poly1;
    poly1=VisiLibity::Polygon(vertices_temp);

    VG_Polygon* vgpoly1=VG_PolygonNew(vgroot);

    VG_PolygonVertex(vgpoly1,VP1);
    VG_PolygonVertex(vgpoly1,VP2);
    VG_PolygonVertex(vgpoly1,VP3);

};
