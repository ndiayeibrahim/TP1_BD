#pragma once

#ifdef __cplusplus
extern "C"{
#endif

void raylib_start(void);

#ifdef __cplusplus
}
#endif



// //----ADD JOUEUR
// void add_inscription_joueur(sqlite3* db){

//     if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
//         LOG_SQLITE3_ERROR(db);
//         return;
//     }
//     sqlite3_stmt* stmt = NULL;
//     char content[] =
//     "INSERT INTO joueurs(nom,vie,force,position_id)\n"
//     "VALUES\n"
//     "(?,?,?,?);";
//     int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
//     char *nom = "Ezechiel";
//     int vie = 100;
//     int force = 100;
//     int position_id = 5; //le joueur est d'abord chez lui

//     if(sqlite3_bind_text(stmt,1,nom,-1,SQLITE_STATIC) != SQLITE_OK){
//         LOG_SQLITE3_ERROR(db);
//     }
//     if(sqlite3_bind_int(stmt,2,vie) != SQLITE_OK){
//         LOG_SQLITE3_ERROR(db);
//     }
//     if(sqlite3_bind_int(stmt,3,force) != SQLITE_OK){
//         LOG_SQLITE3_ERROR(db);
//     }
//     if(sqlite3_bind_int(stmt,4,position_id) != SQLITE_OK){
//         LOG_SQLITE3_ERROR(db);
//     }
//     ret = sqlite3_step(stmt);
//     if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
//         LOG_SQLITE3_ERROR(db);
//     }
// }
// //----FIN ADD JOUEUR
