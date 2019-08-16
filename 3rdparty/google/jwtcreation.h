#ifndef JWTCREATION_H
#define JWTCREATION_H

char* CreateJwt(const char* ec_private_path, const char* project_id, const char *algorithm);

#endif // JWTCREATION_H
