#include "entrypoint.h"
#include "sqlite3.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

#define LOG_SQLITE3_ERROR(db) knob_log(KNOB_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))
// Fonction pour créer la base de données et les tables
void create_data_base(sqlite3* db){

    char* sql = "CREATE TABLE IF NOT EXISTS lieux(id PRIMARY KEY, nom TEXT, description TEXT);";
    int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }

    sql = "CREATE TABLE IF NOT EXISTS objets(id PRIMARY KEY, nom TEXT, description TEXT, id_Lieu INTEGER);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }

    sql = "CREATE TABLE IF NOT EXISTS joueurs(id PRIMARY KEY, nom TEXT,vie INTEGER, force INTEGER, position_id INTEGER);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }

    sql = "CREATE TABLE IF NOT EXISTS pnj( id PRIMARY KEY, nom TEXT, description TEXT, id_lieu INTEGER, dialogue TEXT);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }

    sql = "CREATE TABLE IF NOT EXISTS ennemis( id PRIMARY KEY, nom TEXT, vie INTEGER, force INTEGER, id_lieu INTEGER);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }

    sql = "CREATE TABLE IF NOT EXISTS inventaire( id PRIMARY KEY,id_joueur , id_objet INTEGER );";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }
    sql = "CREATE TABLE IF NOT EXISTS quetes(id PRIMARY KEY, description TEXT, est_complete INTEGER, id_joueur INTEGER, id_pnj INTEGER);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }
    sql = "CREATE TABLE IF NOT EXISTS pnj_quetes(id PRIMARY KEY, id_pnj INTEGER, id_quete INETEGR);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }
}
// Fonction pour afficher ,les informations des lieux

void see_state_fruits(sqlite3* db,char* state_name){
    sqlite3_stmt* stmt = NULL;
    char content[] = "SELECT* FROM lieux;";
   
    
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);

    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
// Cette Boucle je l'ai mis grace a Chapgpt qui permet de parcourir les lignes d'un résultat de requete SQLite
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* nom = sqlite3_column_text(stmt, 1);
        const unsigned char* description = sqlite3_column_text(stmt, 2);
        knob_log(KNOB_INFO, "Lieu : %s (%s)", nom, description);
    }

    sqlite3_finalize(stmt);
}


const char* lieux[] = {
    "Mine Hantée",
    "Foret Sombre",
    "Village Abandonné",
    "Caverne de Glace",
    "Montagne de Feu"
};

const char* descriptionslieu[] = {
    "Une mine abandonnée ou les zombies errent, les murs sont couvertes de poussiers et les piliers sont instables.il y'a des bruit étranges qui viennet des profondeurs de la mine.",
    "Une foret dense et sombre ou les createurs de la nuit se cachent, les arbres sont haut et les feuilles sont épaisses. Il y'a des ruisseaux qui coulent a travers la foret et des oiseaux qui chantent dans les arbres.",
    "Une village abandonnée ou les survivants ont laissé derriére eux leurs biens, les maisons sont vides et les rues sont désertes. Il y'a des débris qui couvrent les rues et des portes qui sont ouvertes.",
    "Une caverne de Glace ou les ours polaires les loups se battent pour survivre, les murs sont couvertes de glaces et les sols sont Glissants. Il y'a des aiguilles qui pendent du plafond et des piliers qui montent du sol.",
    "Une Montagne de feu ou les dragons et les démons se battent pour dominer, les roches sont brulantes et les flames sont hautes, Il y'a des explosions qui retentissent dans la montagne et des éclairs qui illuminent le ciel."
    
};
const char* objets[] = {
    "Hache",
    "Pioche",
    "Torche",
    "Potion de vie",
    "Épée"
};
const char* objets_descriptions[] = {
    "Une hache tranchante qui permet de couper les arbres et les ennemis, elle est lourde et difficile a manier mais trés efficace.",
    "Une pioche solide qui peut creuser les sols et les roches, elle est utiles pour trouver des ressources et des trésors.",
    "Une torche qui éclaire les ténébres et les cavernes, elle est utile pour voir les ennemis et les dangers qui se cachent dans l'obscrité.",
    "Une potion de vie qui restaure les points de vie et les forces , elle est utile pour survivre aux combats et aux dangers.",
    "Une Épée tranchante qui peut tuer les ennemis et les montres, elle est légére et facile a manier mais trés éfficace."

};
// // Fonction pour ajouter des objets , des ennemis et des requetes
void add_objets(sqlite3* db){
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return;
    }
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO objets (nom, description, id_lieu)\n"
    "VALUES\n"
    "(?,?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    // int objet = rand() %5;
    // int lieu = rand() %5;
    // char* nom = "Objet";
    // char* description = "Description";
    // double price = ((rand()+1) % 100) /100 + rand() % 2;
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
       
    }
    for(int i = 0; i < 5; i++){
        if(sqlite3_bind_text(stmt,1,objets[i],-1,SQLITE_STATIC) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        if(sqlite3_bind_text(stmt,2,objets_descriptions[i],-1,SQLITE_STATIC) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        if(sqlite3_bind_int(stmt,3, i + 1) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        ret = sqlite3_step(stmt);
        sqlite3_reset(stmt);

    }
    sqlite3_finalize(stmt);
    // if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
    //     LOG_SQLITE3_ERROR(db);
    // }
}


void add_lieux(sqlite3*db){
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO lieux ( nom, description)\n"
    "VALUES\n"
    "(?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
    }
    for(int i = 0; i < 5; i++){
        if(sqlite3_bind_text(stmt,1,lieux[i],-1,SQLITE_STATIC) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        if(sqlite3_bind_text(stmt,2,descriptionslieu[i],-1,SQLITE_STATIC) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        ret = sqlite3_step(stmt);
        sqlite3_reset(stmt);

    }
    sqlite3_finalize(stmt);
   

}

void add_ennemis (sqlite3* db){
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO ennemis ( nom, vie, force, id_lieu)\n"
    "VALUES\n"
    "(?,?,?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
    }
    const char* ennemis[] = {
        "Zombie",
        "Goblin",
        "Troll",
        "Dragon",
        "Orc"
    };
    int vie[] = { 50, 75, 100, 150, 200};
    int force[] = { 10, 15, 20, 25, 30};
    for (int i = 0; i < 5; i++)
    {
        if(sqlite3_bind_text(stmt,1,ennemis[i],-1,SQLITE_STATIC) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        if(sqlite3_bind_int(stmt,2,vie[i]) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        if(sqlite3_bind_int(stmt,3,force[i]) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        
        if(sqlite3_bind_int(stmt,4,i + 1) != SQLITE_OK){
            LOG_SQLITE3_ERROR(db);
        }
        ret = sqlite3_step(stmt);
        if( ret != SQLITE_DONE){
            LOG_SQLITE3_ERROR(db);
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
}

void add_quetes (sqlite3*db, char* description, int est_complete, int id_joueur, int id_pnj){
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO quetes ( description, est_complete, id_joueur, id_pnj)\n"
    "VALUES\n"
    "(?,?,?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_text(stmt,1,description,-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,2,est_complete) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,3,id_joueur) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    
    if(sqlite3_bind_int(stmt,4,id_pnj) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

}
void add_inscription_joueur(sqlite3* db){
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO joueurs (nom, vie, force, position_id)\n"
    "VALUES\n"
    "(?,?,?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    char* nom = "Ibrahima";
    int vie = 150;
    int force = 150;
    int position_id = 7;
    if(sqlite3_bind_text(stmt,1,nom,-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,2,vie) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,3,force) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,4,position_id) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    ret = sqlite3_step(stmt);
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }
    


}

void afficher_lieux(sqlite3* db){
    sqlite3_stmt* stmt;
    const char* requete = "SELECT id , nom FROM lieux;"; // requete pour obtenir les lieux
    if (sqlite3_prepare_v2(db, requete, -1, &stmt, NULL) == SQLITE_OK)
    {
        printf("Lieux disponibles :\n");
        while(sqlite3_step(stmt) == SQLITE_ROW){
            int id = sqlite3_column_int(stmt, 0);
            const char* nom = (const char*)sqlite3_column_text(stmt, 1);
            printf("%d. %s\n", id, nom); // Afficher l'id et le nom du lieux 
        }
    }
    sqlite3_finalize(stmt);
    


}

void explorer_lieux(sqlite3* db, int id_joueur){
    sqlite3_stmt* stmt;
    const char* requete = "SELECT id , nom, description FROM lieux;";
    int ret = sqlite3_prepare_v2(db,requete,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    printf("\nLieux disponibles :\n");
    while(sqlite3_step(stmt) == SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        const char* nom = (const char*)sqlite3_column_text(stmt, 1);
        printf("%d. %s\n", id, nom); // Afficher l'id et le nom du lieux 
    }
    sqlite3_finalize(stmt);
    printf("\nEntrez l'ID du lieux ou vous voulez aller : ");
    int choix;
    scanf("%d", &choix);

    //Verifier si le lieux existe 
    requete = "SELECT description FROM lieux WHERE id = ?;";
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    sqlite3_bind_int(stmt,1,choix);
    if(sqlite3_step(stmt) == SQLITE_ROW){
        const char* description = (const char*)sqlite3_column_text(stmt, 0);

        char update_sql[] = "UPDATE joueurs SET position_id = ? WHERE id = ?;";
        sqlite3_stmt* update_stmt;
        if(sqlite3_prepare_v2(db,update_sql,-1,&update_stmt,NULL) == SQLITE_OK){
            sqlite3_bind_int(update_stmt, 1, choix);
            sqlite3_bind_int(update_stmt, 2, id_joueur);
            if(sqlite3_step(update_stmt) != SQLITE_DONE){
                LOG_SQLITE3_ERROR(db);
            }
            // sqlite3_finalize(update_stmt);
        }
        printf("\nVous vous etes deplacer vers : %d. %s\n", choix, description);
        
    }
    else {
        printf("\nLieu invalide. \n");
    }
    sqlite3_finalize(stmt);

    

}
void afficher_inventaire(sqlite3* db, int id_joueur){
    sqlite3_stmt* stmt;
    const char* requete_inventaire = "SELECT id_objets FROM inventaire WHERE id_joueur = ?;";
    if (sqlite3_prepare_v2(db, requete_inventaire, -1, &stmt, NULL) == SQLITE_OK){
        sqlite3_bind_int(stmt,1,id_joueur);
        printf("Inventaire du joueur %d :\n", id_joueur);
        while(sqlite3_step(stmt) == SQLITE_ROW){
            int id_objet = sqlite3_column_int(stmt, 0);
            sqlite3_stmt* stmt_objet;
            const char* requete_objet = "SELECT nom FROM objets WHERE id = ?;";
            if (sqlite3_step(stmt_objet) == SQLITE_ROW){
                const char* nom_objet = (const char*)sqlite3_column_text(stmt_objet,0);
                printf("- %s\n", nom_objet);

            }
            sqlite3_finalize(stmt_objet);


        }
    }
    sqlite3_finalize(stmt);
    


}

void Combat(sqlite3* db, int id_joueur, int id_ennemi){
    sqlite3_stmt* stmt;
    const char* content = "SELECT vie, force FROM joueurs WHERE id = ?;";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
    }
    if(sqlite3_bind_int(stmt,1,id_joueur) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    int vie_joueur = 0, force_joueur = 0;
    if ((ret = sqlite3_step(stmt)) == SQLITE_ROW){
        vie_joueur = sqlite3_column_int(stmt, 0);
        force_joueur =  sqlite3_column_int(stmt, 1);

    }
    sqlite3_finalize(stmt);
    
    // Recupérer les Stats des ennemis
    stmt = NULL;
    content = " SELECT vie, force FROM ennemis WHERE id = ?;";
    ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
    }
    if(sqlite3_bind_int(stmt,1,id_ennemi) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    int vie_ennemi = 0, force_ennemi = 0;
    if ((ret = sqlite3_step(stmt)) == SQLITE_ROW){
        vie_ennemi = sqlite3_column_int(stmt, 0);
        force_ennemi =  sqlite3_column_int(stmt, 1);

    }
    sqlite3_finalize(stmt);

    // Cette Boucle de combat permet de gerer le syteme de combat entre le joueur et un ennemis dans le jeu que j'ai eu grace Duck.ia
    while (vie_joueur > 0 && vie_ennemi > 0) {
        // Joueur attaque
        vie_ennemi -= force_joueur;
        knob_log(KNOB_INFO, "Vous attaquez l'ennemi ! Il lui reste %d points de vie.", vie_ennemi);

        if (vie_ennemi <= 0) {
            knob_log(KNOB_INFO, "Vous avez vaincu l'ennemi !");
            break;
        }

        // Ennemi attaque
        vie_joueur -= force_ennemi;
        knob_log(KNOB_INFO, "L'ennemi vous attaque ! Il vous reste %d points de vie.", vie_joueur);

        if (vie_joueur <= 0) {
            knob_log(KNOB_INFO, "Vous avez été vaincu par l'ennemi !");
            break;
        }
    }
}

void Update_requete(sqlite3* db, int id_quete, int est_complete){
    sqlite3_stmt* stmt;
    const char* content = "UPDATE quetes SET est_complete = ? WHERE id = ?;";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
    }
    if(sqlite3_bind_int(stmt,1,est_complete) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,2,id_quete) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    ret = sqlite3_step(stmt);
    if(ret != SQLITE_DONE){
        LOG_SQLITE3_ERROR(db);
    }
    sqlite3_finalize(stmt);
    

}

void interagir_pnj(sqlite3* db, int id_pnj, int id_quete){
    sqlite3_stmt* stmt;
    const char* content = "INSERT INTO pnj_quetes(id_pnj, id_quete) VALUES (?, ?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
        return;
    }
    if(sqlite3_bind_int(stmt,1,id_pnj) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_int(stmt,2,id_quete) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    ret = sqlite3_step(stmt);
    if(ret != SQLITE_DONE){
        LOG_SQLITE3_ERROR(db);
    }
    sqlite3_finalize(stmt);
    
}



void raylib_start(void){
    srand(time(NULL));
    sqlite3* db = NULL;
    sqlite3_open("./aventure_quete.db",&db);
    create_data_base(db);
    afficher_lieux(db);
    // see_state_fruits(db,"lieu");
    add_lieux(db);
    add_objets(db);
    add_ennemis(db);
    add_inscription_joueur(db);
    int id_joueur = 1;
    explorer_lieux(db, id_joueur);
    int id_joueurs = 1;
    afficher_inventaire(db, id_joueur);
    int id_ennemi = 1;
    Combat(db, id_joueur, id_ennemi);
    int id_quete = 1, est_complete = 1;

    Update_requete(db, id_quete, est_complete);
    int id_pnj = 1;
    interagir_pnj(db,id_pnj,id_quete);

    // add_quete(db,"Quete 1", 0, 1, 1);
    // add_random_fruit(db);//Uncomment to add a random fruit...
    
    sqlite3_close(db);
    return;
}