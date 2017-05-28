#ifndef TIMING_H
#define TIMING_H

#include<iostream>
#include<iomanip>
#include<QDate>
#include<string>
#include<sstream>

namespace TIME {
    /*! \class TimeException
            \brief Classe permettant de gérer les exceptions des classes du namespace TIME
    */
    class TimeException{
    public:
        //! Constructeur à partir d'une string

        TimeException(const std::string& m):info(m){}
        //! Retourne l'information stockée dans la classe
        const std::string& GetInfo() const { return info; }
    private:
        std::string info;
    };

    /*! \class Date
            \brief Classe permettant de manipuler des dates standards
            L'utilisation de cette classe nécessite des dates valides au sens commun du terme.
            Déclenchement d'exception dans le cas contraire
    */
    class Date {
    public:
        //! Constructeur à partir d'un jour, mois, année

        /*! \param j jour avec 1<=j<=31
            \param m mois avec 1<=m<=12
            \param a ann�e avec a>=0
            */
        Date(unsigned int short j=1, unsigned int short m=1, unsigned int a=0):jour(1),mois(1),annee(0){ setDate(j,m,a); }
        // m�thodes
        //! Retourne le jour de la date
        unsigned short int  getJour() const { return jour; }
        //! Retourne le mois de la date
        unsigned short int  getMois() const { return mois; }
        //! Retourne l'ann�e de la date
        unsigned int getAnnee() const { return annee; }
        //! Retourne l'ann�e de la date
        QDate getQDate() const { return QDate(annee, mois, jour); }
        std::string getJourMoisString() const {
            std::stringstream sstm;
            sstm << jour << '/'<< mois;
            return sstm.str();
        //!Retourne l'heure sous forme de chaine
        }
        //!< initialisation de la date
        void setDate(unsigned short int j, unsigned short int m, unsigned int a);
        //!< initialisation de la date avec la date d'aujourd'hui
        void setDateAujourdhui();
        //!< affiche le date sous le format JJ/MM/AAAA
        void afficher(std::ostream& f=std::cout) const;
        //! d1==d2 retourne vrai si les deux dates sont �gales
        bool operator==(const Date& d) const;
        //! Compare deux dates dans le temps : d1<d2 retourne true si d1 est avant d2
        bool operator<(const Date& d) const;
        //! Compare deux dates dans le temps : d1<=d2 retourne true si d1 est avant ou bien �gale � d2
        bool operator<=(const Date& d) const;
        //! Retourne le nombre de jours s�parant les deux dates
        int operator-(const Date& d) const;
        //! Retourne la date du lendemain
        Date demain() const;
        //!Retourne la date de dans nb jours
        Date operator+(unsigned int nb) const;
        static Date toTimingDate(QDate d);
    private:
        // attributs
        unsigned short int jour; // jour entre 1 et 31
        unsigned short int mois; // mois entre 1 et 12
        unsigned int annee;
    };

    /*! \class Horaire
            \brief Classe permettant de manipuler des horaires
            L'utilisation de cette classe n�cessite des dates valides au sens commun du terme.
            D�clenchement d'exception dans le cas contraire
    */
    class Horaire{
    public:
        //! Constructeur � partir de heure et minute

        /*! \param h heure avec 0<=h<=23
            \param m minute avec 0<=m<=59
            */
        Horaire(unsigned short int  h, unsigned short int  m):heure(h),minute(m) {
            while(m>59){
                h++;
                m=m%60;
            }
            if (h>23||m>59) throw TimeException("erreur: initialisation horaire invalide");
        }
        void setHoraire(unsigned short int h, unsigned short int m) { if (h>23||m>59) throw TimeException("erreur: initialisation horaire invalide"); heure=h; minute=m; }
        //!Affiche l'horaire sous le format hhHmm
        void afficher(std::ostream& f=std::cout) const { f<<std::setfill('0')<<std::setw(2)<<heure<<"H"<<std::setfill('0')<<std::setw(2)<<minute<<std::setfill(' '); }
        std::string toString() const {
            std::string ret;
            std::stringstream sstm;
            sstm <<std::setfill('0')<<std::setw(2)<<heure<<"h"<<std::setfill('0')<<std::setw(2)<<minute<<std::setfill(' ');
            ret = sstm.str();
            return ret;
        //!Retourne l'heure sous forme de chaine
        }
        const Horaire operator+( const Duree& d) const { return Horaire((heure+d.getDureeEnHeuresInt())%24, minute+d.getDureeEnMinutes()-d.getDureeEnHeuresInt()*60); }
        //!Retourne l'heure de l'horaire
        unsigned short int getHeure() const { return heure; }
        //!Retourne les minutes de l'horaire
        unsigned short int getMinute() const { return minute; }
        //! h1<h2 retourne true si h1 est avant h2 dans le temps
        bool operator<(const Horaire& h) const;
    private:
        unsigned short int  heure;
        unsigned short int  minute;
    };

}

std::ostream& operator<<(std::ostream&, const TIME::Date&);
std::ostream& operator<<(std::ostream& f, const TIME::Horaire & h);

std::istream& operator>>(std::istream&, TIME::Date&); // lecture format JJ/MM/AAAA
std::istream& operator>>(std::istream&, TIME::Duree&); //lecture format hhHmm

#endif // TIMING_H
