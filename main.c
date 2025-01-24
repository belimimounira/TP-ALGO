#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définition de la structure Patient
typedef struct Patient {
    int id;
    char nom[50];
    char prenom[50];
    int age;
    char adresse[100];
    char groupeSanguin[5];
} Patient;

// Définition de la structure Patienturgence
typedef struct Patienturgence {
    int id;
    char nom[50];
    char prenom[50];
    int age;
    char adresse[100];
    char groupeSanguin[5];
    int priorite; // Niveau d'urgence : 1 = Élevée, 2 = Moyenne, 3 = Faible
} Patienturgence;

// Définition d'un nœud de la file pour les patients réguliers
typedef struct Node {
    Patient patient;
    struct Node *suivant;
} Node;

// Définition d'un nœud de la file pour les patients urgents
typedef struct NodeUrgence {
    Patienturgence patient;
    struct NodeUrgence *suivant;
} NodeUrgence;

// Définition de la file pour les patients réguliers
typedef struct Queue {
    Node *avant;
    Node *arriere;
} Queue;

// Définition de la file pour les patients urgents
typedef struct QueueUrgence {
    NodeUrgence *avant;
    NodeUrgence *arriere;
} QueueUrgence;

// Définition de la structure Consultation
typedef struct Consultation {
    int idPatient;
    char date[20];       // Date de consultation (ex : "2025-01-23")
    char medecin[50];    // Nom du médecin
    char motif[100];     // Motif de la consultation
} Consultation;

// Définition de l'historique des consultations
typedef struct NodeConsultation {
    Consultation consultation;
    struct NodeConsultation *suivant;
} NodeConsultation;

typedef struct Historique {
    NodeConsultation *tete; // Premier élément de l'historique
} Historique;

// Initialisation de la file des patients réguliers
void initialiserQueue(Queue *queue) {
    queue->avant = NULL;
    queue->arriere = NULL;
}

// Initialisation de la file des patients urgents
void initialiserQueueUrgence(QueueUrgence *queue) {
    queue->avant = NULL;
    queue->arriere = NULL;
}

// Initialisation de l'historique des consultations
void initialiserHistorique(Historique *historique) {
    historique->tete = NULL;
}

// Vérifie si la file des patients réguliers est vide
int estVide(Queue *queue) {
    return queue->avant == NULL;
}

// Vérifie si la file des patients urgents est vide
int estVideUrgence(QueueUrgence *queue) {
    return queue->avant == NULL;
}

// Ajout d'un patient à la file des patients réguliers
void ajouterPatient(Queue *queue, Patient patient) {
    Node *nouveauNode = (Node *)malloc(sizeof(Node));
    if (!nouveauNode) {
        printf("Erreur d'allocation mémoire.\n");
        return;
    }
    nouveauNode->patient = patient;
    nouveauNode->suivant = NULL;

    if (estVide(queue)) {
        queue->avant = nouveauNode;
        queue->arriere = nouveauNode;
    } else {
        queue->arriere->suivant = nouveauNode;
        queue->arriere = nouveauNode;
    }

    printf("Patient ajouté : %s %s\n", patient.nom, patient.prenom);
}

// Ajout d'un patient à la file des patients urgents avec tri par priorité
void ajouterPatientUrgence(QueueUrgence *queue, Patienturgence patient) {
    NodeUrgence *nouveauNode = (NodeUrgence *)malloc(sizeof(NodeUrgence));
    if (!nouveauNode) {
        printf("Erreur d'allocation mémoire.\n");
        return;
    }
    nouveauNode->patient = patient;
    nouveauNode->suivant = NULL;

    if (queue->avant == NULL || patient.priorite < queue->avant->patient.priorite) {
        nouveauNode->suivant = queue->avant;
        queue->avant = nouveauNode;
    } else {
        NodeUrgence *temp = queue->avant;
        while (temp->suivant != NULL && temp->suivant->patient.priorite <= patient.priorite) {
            temp = temp->suivant;
        }
        nouveauNode->suivant = temp->suivant;
        temp->suivant = nouveauNode;
    }
    printf("Patient urgent ajouté : %s %s avec priorité %d\n", patient.nom, patient.prenom, patient.priorite);
}

// Suppression d'un patient (retirer le premier de la file des réguliers)
Patient retirerPatient(Queue *queue) {
    Patient patientVide = {0, "", "", 0, "", ""}; // Patient vide par défaut

    if (estVide(queue)) {
        printf("La file est vide, aucun patient à retirer.\n");
        return patientVide;
    }

    Node *temp = queue->avant;
    Patient patient = temp->patient;
    queue->avant = queue->avant->suivant;

    if (queue->avant == NULL) {
        queue->arriere = NULL;
    }

    free(temp);
    printf("Patient retiré : %s %s\n", patient.nom, patient.prenom);
    return patient;
}

// Afficher tous les patients dans la file des réguliers
void afficherQueue(Queue *queue) {
    if (estVide(queue)) {
        printf("La file est vide.\n");
        return;
    }

    printf("\n--- Liste des Patients dans la file ---\n");
    Node *temp = queue->avant;
    while (temp) {
        Patient p = temp->patient;
        printf("ID: %d, Nom: %s, Prénom: %s, Âge: %d, Adresse: %s, Groupe Sanguin: %s\n",
               p.id, p.nom, p.prenom, p.age, p.adresse, p.groupeSanguin);
        temp = temp->suivant;
    }
}

// Afficher tous les patients dans la file des urgents
void afficherQueueUrgence(QueueUrgence *queue) {
    if (estVideUrgence(queue)) {
        printf("La file des urgents est vide.\n");
        return;
    }

    printf("\n--- Liste des Patients Urgents ---\n");
    NodeUrgence *temp = queue->avant;
    while (temp) {
        Patienturgence p = temp->patient;
        printf("ID: %d, Nom: %s, Prénom: %s, Âge: %d, Adresse: %s, Groupe Sanguin: %s, Priorité: %d\n",
               p.id, p.nom, p.prenom, p.age, p.adresse, p.groupeSanguin, p.priorite);
        temp = temp->suivant;
    }
}

// Ajouter une consultation à l'historique
void ajouterConsultation(Historique *historique, Consultation consultation) {
    NodeConsultation *nouveauNode = (NodeConsultation *)malloc(sizeof(NodeConsultation));
    if (!nouveauNode) {
        printf("Erreur d'allocation mémoire.\n");
        return;
    }
    nouveauNode->consultation = consultation;
    nouveauNode->suivant = NULL;

    // Ajouter la nouvelle consultation au début de la liste
    if (historique->tete == NULL) {
        historique->tete = nouveauNode;
    } else {
        nouveauNode->suivant = historique->tete;
        historique->tete = nouveauNode;
    }

    printf("Consultation ajoutée pour le patient ID %d : %s avec le Dr %s\n",
           consultation.idPatient, consultation.date, consultation.medecin);
}

// Afficher l'historique des consultations
void afficherHistorique(Historique *historique) {
    if (historique->tete == NULL) {
        printf("L'historique des consultations est vide.\n");
        return;
    }

    printf("\n--- Historique des consultations ---\n");
    NodeConsultation *temp = historique->tete;
    while (temp != NULL) {
        Consultation c = temp->consultation;
        printf("ID Patient: %d, Date: %s, Médecin: %s, Motif: %s\n",
               c.idPatient, c.date, c.medecin, c.motif);
        temp = temp->suivant;
    }
}

int main() {
    Queue queue, rqueue;
    QueueUrgence urgenceQueue;
    initialiserQueue(&queue);
    initialiserQueue(&rqueue);
    initialiserQueueUrgence(&urgenceQueue);
    Historique historique;
    initialiserHistorique(&historique);

    int choix;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Ajouter un patient\n");
        printf("2. Retirer un patient\n");
        printf("3. Afficher les patients\n");
        printf("4. Ajouter un patient urgent\n");
        printf("5. Historique de consultation\n");
        printf("6. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                Patient patient;
                printf("\n--- Ajouter un Patient ---\n");
                printf("ID : ");
                scanf("%d", &patient.id);
                printf("Nom : ");
                scanf("%s", patient.nom);
                printf("Prénom : ");
                scanf("%s", patient.prenom);
                printf("Âge : ");
                scanf("%d", &patient.age);
                printf("Adresse : ");
                getchar(); // Consommer le retour à la ligne
                fgets(patient.adresse, 100, stdin);
                patient.adresse[strcspn(patient.adresse, "\n")] = '\0';
                printf("Groupe Sanguin : ");
                scanf("%s", patient.groupeSanguin);

                ajouterPatient(&queue, patient);
                break;
            }
            case 2:
                retirerPatient(&queue);
                break;
            case 3:
                afficherQueue(&queue);
                break;
            case 4: {
                Patienturgence patientUrgence;
                printf("\n--- Ajouter un Patient Urgent ---\n");
                printf("ID : ");
                scanf("%d", &patientUrgence.id);
                printf("Nom : ");
                scanf("%s", patientUrgence.nom);
                printf("Prénom : ");
                scanf("%s", patientUrgence.prenom);
                printf("Âge : ");
                scanf("%d", &patientUrgence.age);
                printf("Adresse : ");
                getchar(); // Consommer le retour à la ligne
                fgets(patientUrgence.adresse, 100, stdin);
                patientUrgence.adresse[strcspn(patientUrgence.adresse, "\n")] = '\0';
                printf("Groupe Sanguin : ");
                scanf("%s", patientUrgence.groupeSanguin);
                printf("Priorité (1 = Élevée, 2 = Moyenne, 3 = Faible) : ");
                scanf("%d", &patientUrgence.priorite);

                ajouterPatientUrgence(&urgenceQueue, patientUrgence);
                break;
            }
            case 5: {
                int choixHist;
                do {
                    printf("\n--- Menu Historique des Consultations ---\n");
                    printf("1. Ajouter une consultation\n");
                    printf("2. Afficher l'historique des consultations\n");
                    printf("3. Retour au menu principal\n");
                    printf("Choix : ");
                    scanf("%d", &choixHist);

                    switch (choixHist) {
                        case 1: {
                            Consultation consultation;
                            printf("\n--- Ajouter une Consultation ---\n");
                            printf("ID du Patient : ");
                            scanf("%d", &consultation.idPatient);
                            printf("Date (AAAA-MM-JJ) : ");
                            scanf("%s", consultation.date);
                            printf("Nom du Médecin : ");
                            scanf("%s", consultation.medecin);
                            getchar(); // Consommer le retour à la ligne
                            printf("Motif de la consultation : ");
                            fgets(consultation.motif, 100, stdin);
                            consultation.motif[strcspn(consultation.motif, "\n")] = '\0';

                            ajouterConsultation(&historique, consultation);
                            break;
                        }
                        case 2:
                            afficherHistorique(&historique);
                            break;
                        case 3:
                            break;
                        default:
                            printf("Choix invalide, veuillez réessayer.\n");
                    }
                } while (choixHist != 3);
                break;
            }
            case 6:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choix != 6);

    return 0;
}
