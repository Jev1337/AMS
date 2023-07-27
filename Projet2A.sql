--BEGIN

  --Bye Tables!
  --FOR i IN (SELECT ut.table_name
  --            FROM USER_TABLES ut) LOOP
  --  EXECUTE IMMEDIATE 'drop table '|| i.table_name ||' CASCADE CONSTRAINTS ';
  --END LOOP;

--END;

CREATE TABLE AccountType (
  type NUMBER(10) PRIMARY KEY,
  description VARCHAR2(255)
);

CREATE TABLE VEHICLE (
  VITESSE NUMBER(10) 
);

CREATE TABLE Heartrate (
  pulsation float(10)
);

CREATE TABLE CommunicationCenter (
  id NUMBER(10) PRIMARY KEY,
  nom VARCHAR2(255),
  adresse VARCHAR2(255),
  num_tel VARCHAR2(20),
  email VARCHAR2(255),
  remarques VARCHAR2(255),
  created_at TIMESTAMP,
  updated_at TIMESTAMP
);

CREATE TABLE Ambulance (
  matricule VARCHAR2(20) PRIMARY KEY,
  etat VARCHAR2(255),
  capacite NUMBER(10),
  emplacement VARCHAR2(255),
  equipements VARCHAR2(255),
  remarques VARCHAR2(255),
  created_at TIMESTAMP,
  updated_at TIMESTAMP
);

CREATE TABLE Hospital (
  id NUMBER(10) PRIMARY KEY,
  nom_dep VARCHAR2(255),
  adresse VARCHAR2(255),
  numero VARCHAR2(20),
  email VARCHAR2(255),
  capacite NUMBER(10),
  service VARCHAR2(255),
  equipements VARCHAR2(255),
  remarques VARCHAR2(255),
  created_at TIMESTAMP,
  updated_at TIMESTAMP
);

CREATE TABLE Patient (
  CIN VARCHAR2(20) PRIMARY KEY,
  nom VARCHAR2(255),
  prenom VARCHAR2(255),
  date_naissance VARCHAR2(255),
  created_at TIMESTAMP,
  updated_at TIMESTAMP
);

CREATE TABLE HistoriquePatient (
  CIN VARCHAR2(20),
  Libelle VARCHAR2(255),
  description VARCHAR2(255),
  PRIMARY KEY (CIN, Libelle),
  FOREIGN KEY (CIN) REFERENCES Patient(CIN)
);

CREATE TABLE Employee (
  CIN VARCHAR2(20) PRIMARY KEY,
  type NUMBER(10) REFERENCES AccountType(type),
  mail VARCHAR2(255),
  password VARCHAR2(255),
  nom VARCHAR2(255),
  prenom VARCHAR2(255),
  date_naissance VARCHAR2(255),
  face_id VARCHAR2(255),
  face_TS TIMESTAMP,
  fingerprint_id VARCHAR2(3),
  fingerprint_date TIMESTAMP,
  isadmin NUMBER(1),
  phone VARCHAR2(8),
  remarques VARCHAR2(255),
  created_at TIMESTAMP,
  updated_at TIMESTAMP
);

CREATE TABLE EmergencyCall (
  id NUMBER(10) PRIMARY KEY,
  patient_cin VARCHAR2(20),
  communication_center_id NUMBER(10),
  matricule_ambulance VARCHAR2(20),
  hospital_id NUMBER(10),
  problem VARCHAR2(255),
  adresse VARCHAR2(255),
  status NUMBER(10),
  remarks VARCHAR2(255),
  created_at TIMESTAMP,
  updated_at TIMESTAMP
);
CREATE SEQUENCE dept_seq START WITH 1;
CREATE OR REPLACE TRIGGER dept_bir 
BEFORE INSERT ON emergencycall
FOR EACH ROW

BEGIN
  SELECT dept_seq.NEXTVAL
  INTO   :new.id
  FROM   dual;
END;
/

INSERT INTO AccountType (type, description) VALUES (1, 'administrator');
INSERT INTO AccountType (type, description) VALUES (2, 'comms_emp');
INSERT INTO AccountType (type, description) VALUES (3, 'hospital_emp');
INSERT INTO AccountType (type, description) VALUES (4, 'patientmgr_emp');
INSERT INTO AccountType (type, description) VALUES (5, 'ambulance_emp');