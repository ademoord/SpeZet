#
#   createSpeZet.sql
#
#   create the database from the very beginning
#
DROP DATABASE IF EXISTS SpeZet;     # delete SpeZet database if available
CREATE DATABASE SpeZet;             # create the database
USE SpeZet;                         # use it

CREATE TABLE tbJob (
    idJob INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
    shortJob VARCHAR(6) NOT NULL,
    Job VARCHAR(30) NOT NULL,
    Mode VARCHAR(1)
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;
    
    
CREATE TABLE cnvJob (
    idJobOld INTEGER NOT NULL PRIMARY KEY,
    idJobNew INTEGER
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE tbEmployee (
    idEmployee INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
    shortName VARCHAR(6) NOT NULL,
    Name VARCHAR(40) NOT NULL,
    dtBegin DATETIME NOT NULL,
    dtEnd DATETIME,
    Mode VARCHAR(1),
    PartTime FLOAT,
    idJob INTEGER
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;
        

CREATE TABLE cnvEmployee (
    idEmplOld INTEGER NOT NULL PRIMARY KEY,
    idEmplNew INTEGER
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE tbOwner (
    idOwner INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
    Company VARCHAR(40) NOT NULL,
    Attention VARCHAR(40),
    Address VARCHAR(40),
    Postcode VARCHAR(5),
    City VARCHAR(25),
    Country VARCHAR(25),
    Mode VARCHAR(1)
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE cnvOwner (
    idOwnOld INTEGER NOT NULL PRIMARY KEY,
    idOwnNew INTEGER
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE tbOrder (
    idOrder INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
    Mode VARCHAR(1) NOT NULL,
    shortOrder VARCHAR(20),
    Description VARCHAR(80),
    idOwner INTEGER DEFAULT 0,
    Budget FLOAT,
    dtBegin DATETIME,
    dtEnd DATETIME,
    artCost VARCHAR(1),
    baseRate INTEGER,
    baseItem INTEGER
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE cnvOrder (
    idOrderOld INTEGER NOT NULL PRIMARY KEY,
    idOrderNew INTEGER
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE tbEmployeeJob (
    idEmployee INTEGER DEFAULT 0,
    idJob INTEGER DEFAULT 0
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE tbRate (
    idOrder INTEGER NOT NULL,
    idJob INTEGER NOT NULL,
    dtBegin DATETIME NOT NULL,
    dtEnd DATETIME,
    Rate FLOAT DEFAULT 0
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


CREATE TABLE tbSubOrder (
    idSubOrder INTEGER NOT NULL PRIMARY KEY,
    idOrder INTEGER NOT NULL,
    shortSubOrder VARCHAR(2),
    SubOrder VARCHAR(80),
    Mode VARCHAR(1)
    )
    ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;


