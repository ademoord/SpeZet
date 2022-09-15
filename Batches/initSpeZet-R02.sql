#
#   createSpeZet.sql
#
#   create the database from the very beginning
#
DROP DATABASE IF EXISTS SpeZet;     # delete SpeZet database if available
CREATE DATABASE SpeZet;             # create the database
USE SpeZet;                         # use it

CREATE TABLE tbProject (
    idProject INTEGER NOT NULL AUTO_INCREMENT,     # id project 
    nrProject VARCHAR(20),          		   # nr project
    descProject VARCHAR(80), 
#    idOwner INTEGER NOT NULL, 
#    Budget FLOAT, 
#    dtBegin DATE,
#    dtEnd DATE, 
#    Mode VARCHAR(1), 
#    ArtCost INTEGER, 
#    BaseRate INTEGER, 
#    BaseItem INTEGER, 
    
    PRIMARY KEY (idProject)
#     FOREIGN KEY (DBegin) 
#    REFERENCES MEmpl(DBegin) ON UPDATE CASCADE ON DELETE RESTRICT
    ) 
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;
    

CREATE TABLE tbSubproject (
    idSubproject INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY, 
    idProject INTEGER NOT NULL, 
    nrSubproject VARCHAR(3), 
    descSubproject VARCHAR(40),
#    IDRate INTEGER, 
#    Mode VARCHAR(1),    

#    PRIMARY KEY (IDRate), 
    FOREIGN KEY (idProject) REFERENCES tbProject(idProject)
    ON UPDATE CASCADE ON DELETE RESTRICT)
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


# CREATE TABLE tbTime (               # store the entries
#    idEmployee INTEGER,              # the employee identity
#    idJob INTEGER,                   # job identity
#    idSubproject INTEGER,            # subproject
#    dtBegin DATETIME,                # work starting time
#    dtEnd DATETIME,                  # work end time
#    Description VARCHAR(80)          # work description
#    ) 
#    ENGINE=InnoDB                    # use the InnoDB engine
#    DEFAULT CHARSET=latin1 
#    ROW_FORMAT=FIXED;


CREATE TABLE tbEmployee (
    idEmployee INTEGER NOT NULL AUTO_INCREMENT,
    ShortName VARCHAR(6),
    Name VARCHAR(20),
    dtBegin DATETIME,
    dtEnd DATETIME,
    Directory VARCHAR(30),
    Mode VARCHAR(1),
    PartTime FLOAT,
    idJob INTEGER,

    PRIMARY KEY (idEmployee, idJob)
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;
    
    

CREATE TABLE cnvEmployee (
    idEmplNew INTEGER,
    idEmplOld INTEGER NOT NULL PRIMARY KEY
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;




    
  