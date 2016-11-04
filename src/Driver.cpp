#include "Driver.hpp"
#include <iostream>
#include <ogr_spatialref.h>

using namespace std;
using namespace gps_base;

Driver::Driver()
{
    this->utm_zone = 32;
    this->utm_north = true;
    createCoTransform();
}

void Driver::createCoTransform()
{
    OGRSpatialReference oSourceSRS;
    OGRSpatialReference oTargetSRS;

    oSourceSRS.SetWellKnownGeogCS("WGS84");
    oTargetSRS.SetWellKnownGeogCS("WGS84");
    oTargetSRS.SetUTM(this->utm_zone, this->utm_north);

    coTransform = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);

    if (coTransform == NULL)
    {
        throw runtime_error("Failed to initialize CoordinateTransform");
    }
}

void Driver::setUtmZone(int zone)
{
    this->utm_zone = zone;
    createCoTransform();
}

void Driver::setUtmNorth(bool north)
{
    this->utm_north = north;
    createCoTransform();
}

int Driver::getUtmZone()
{
    return this->utm_zone;
}

bool Driver::getUtmNorth()
{
    return this->utm_north;
}

bool Driver::convertSolutionToRBS(const gps_base::Solution &solution, base::samples::RigidBodyState &position)
{
    // if there is a valid reading, then write it to position readings port
    if (solution.positionType != gps_base::NO_SOLUTION)
    {
        double la = solution.latitude;
        double lo = solution.longitude;
        double alt = solution.altitude;

        coTransform->Transform(1, &lo, &la, &alt);

        position.time = solution.time;
        position.position.x() = lo - origin.x();
        position.position.y() = la - origin.y();
        position.position.z() = alt - origin.z();
        position.cov_position(0, 0) = solution.deviationLongitude * solution.deviationLongitude;
        position.cov_position(1, 1) = solution.deviationLatitude * solution.deviationLatitude;
        position.cov_position(2, 2) = solution.deviationAltitude * solution.deviationAltitude;

        return true;
    }

    return false;
}
