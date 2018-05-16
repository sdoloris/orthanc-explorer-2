import 'rxjs/add/operator/map';

import {Patient, Modality} from './miscellaneous.classes';
import {Injectable} from '@angular/core';
import {Headers, Http, RequestOptions} from '@angular/http';

import 'rxjs/add/operator/toPromise';



@Injectable()
export class RESTService {

  Server = '/';

  constructor(private http: Http) {
  }

  /*
    This function returns an array of Patient
    Parameters:
      -nbOfPatients: the maximal size of the array
      -from: the index of the first element. because the array returned is a
             sublist of the Patient list contained in the server
  */
  public getPatients(nbOfPatient: number, from: number): Promise<any>
  {
    let apiURL = this.Server + 'patients?&since=' + from.toString() + '&limit=' + nbOfPatient.toString() + '&expand';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns an array of Study
    Parameters:
      -nbOfPatients: the maximal size of the array
      -from: the index of the first element. because the array returned is a
             sublist of the Study list contained in the server
  */
  public getStudies(nbOfStudies: number, from: number): Promise<any>
  {
    let apiURL = this.Server + 'studies?&since=' + from.toString() + '&limit=' + nbOfStudies.toString() + '&expand';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns the parent of a particular element
    Parameter:
      -elementType: the type of element (studies, series, instances)
      -ID: the ID of the element
      -parentType: the type of parent (patient, study, series)
  */
  public getParent(elementType: string, ID: string, parentType: string): Promise<any>
  {
    let apiURL = this.Server + elementType + '/' + ID + '/' + parentType;
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns the children of a particular element
    Parameter:
      -elementType: the type of element ('patients', 'studies', 'series')
      -ID: the ID of the element
  */
  public getChildren(elementType: string, ID: string): Promise<any>
  {
    let childrenType;
    let apiURL = this.Server + elementType + '/' + ID + '/';
    switch(elementType)
    {
      case "patients":
      {
        childrenType = "studies";
        break;
      }
      case "studies":
      {
        childrenType = "series";
        break;
      }
      case "series":
      {
        childrenType = "instances";
      }
    }
    if(childrenType != undefined)
      return this.http.get(apiURL + childrenType).toPromise();
  }

  /*
   This function returns the tags for a particular instance ID
   Parameter:
     -instanceID: the ID of the instance
 */
  public getInstanceInfo(instanceID: string): Promise<any>
  {
    let apiURL = this.Server + 'instances/' + instanceID+ '/simplified-tags';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns an array of string. Each string is the ID of an
    available plugin
  */
  public getPluginsName(): Promise<any>
  {
    let apiURL = this.Server + 'plugins';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns the PluginInfo for a particular plugin ID
    Parameter:
      -pluginID: the ID of the plugin
  */
  public getPluginInfo(pluginID: string)
  {
    let apiURL = this.Server + 'plugins/' + pluginID;
    return this.http.get(apiURL).toPromise();
  }

  /*
  This function returns the list of paths of all the active plugins added to Orthanc
  RETURN:
    HTTP GET response encapsulated in a Promise
   */
  public getPluginPaths() {
    let apiURL = this.Server + 'OE2/pluginPath';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns the Patient for a particular patient ID
    Parameter:
      -patientID: the ID of the patient
  */
  public getPatientInfo(patientID: string)
  {
    let apiURL = this.Server + 'patients/' + patientID;
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns the Study for a particular study ID
    Parameter:
      -studyID: the ID of the study
  */
  public getStudyInfo(studyID: string)
  {
    let apiURL = this.Server + 'studies/' + studyID;
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns the Serie for a particular serie ID
    Parameter:
      -serieID: the ID of the serie
  */
  public getSerieInfo(serieID: string)
  {
    let apiURL = this.Server + 'series/' + serieID;
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns a Statistic that contains the statistic of the server
  */
  public getStatistics()
  {
    let apiURL = this.Server + 'statistics';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function returns a System that contains the information of the server
  */
  public getSystem()
  {
    let apiURL = this.Server + 'system';
    return this.http.get(apiURL).toPromise();
  }

  /*
    Returns the different modalities specified in the config file
    RETURN:
      HTTP get response encapsulated in a promise
  */
  public getModalities()
  {
    let apiURL = this.Server + 'OE2/modalities';
    return this.http.get(apiURL).toPromise();
  }

  /*
  Restarts the Orthanc core
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public restart()
  {
    let apiURL = this.Server + 'tools/reset';
    let body = "";
    return this.http.post(apiURL, body).toPromise();
  }

  /*
    This function return the number of series that match a query ID
    Parameter:
      -queryID: the ID of the query
  */
  public getQueryAnswers(queryID :string)
  {
    let apiURL = this.Server + 'queries/' + queryID + '/answers';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function return the index^th Answer that match a particular queryID and the 'index' pass as argument
    Parameters:
      -queryID: the ID of the query
      -index: the index of the anwer needed in the array of Answer that match
              the query ID
  */
  public getAnswers(queryID :string, index: number)
  {
    let apiURL = this.Server + 'queries/' + queryID + '/answers/' + index.toString() + '/content?simplify';
    return this.http.get(apiURL).toPromise();
  }

  /*
  Get the tags associated to a study
  PARAMETERS:
    -studyID: ID of the study
  RETURN:
    -HTTP get response encapsulated in a promise
   */
  public getTags(studyID: string)
  {
    let apiURL = this.Server + 'OE2/tags/'+studyID;
    return this.http.get(apiURL).toPromise();
  }

  /*
  Get the username and its permissions of the active connected user.
  RETURN:
    HTTP get response encapsulated in a promise
   */
  public getPermission()
  {
    let apiURL = this.Server + 'OE2/userdata';
    return this.http.get(apiURL).toPromise();
  }

  /*
  Disconnect the active user from the plugin.
  RETURN:
    HTTP get response encapsulated in a promise
   */
  public disconnect()
  {
    let apiURL = this.Server + 'OE2/disconnect';
    return this.http.get(apiURL).toPromise();
  }


  /*
    This function deletes an element based on its ID and type and returns the
    correspnding RemainingAncestor
    Parameters:
      -ID: the element ID
      -type: the element type
  */
  public delete(id: string, type: string)
  {
    console.log('Call to delete');
    let apiURL = this.Server + type + '/' + id;
    return this.http.delete(apiURL).toPromise();
  }


  /*
    This function uploads an instance from is content and returns the uploadInfo
    Parameter:
      -content: the content of the instance DICOM file
  */
  public upload(content: string)
  {
    const headers = new Headers({
      'Content-Type': 'application/dicom'
    });
    const options = new RequestOptions({
      headers: headers
    });
    let apiURL = this.Server + 'instances';
    let body =  content;
    return this.http.post(apiURL, body, options).toPromise();
  }

  /*
    This function returns a array of ID (string). Each string is the ID of an
     element that match the query
    Parameters:
      -level: the type of returned element ('Patient', 'Study', 'Series' or
              'Instances')
    Each next parameter can be used to precise the request. Please send '*' if
    no value for the parameter. The server is case insensitive.
      -accessionNumber
      -patientBirthDate => format exemple = "20171116"
      -patientID
      -patientName
      -patientSex => format 'F' femelle 'M' male 'U' unknow 'O' other
      -studyDate => format exemple = "20171116"
                                     (for particular day)
                                     "20171116-"
                                     (from a particular day until today)
                                     "20051116-20111116"
                                     (from a particular day to one another)
      -studyDescription
      -ModalitiesInStudy => format exemple: ["CR" "CT"] (array of string)
                                            ["*"] for no particular value
  */
  public getfilteredElement(level: string,
                            accessionNumber: string,
                            patientBirthDate: string,
                            patientID: string,
                            patientName: string,
                            patientSex: string,
                            studyDate: string,
                            studyDescription: string,
                            modalitiesInStudy: string[],
                            since: number,
                            limit: number)
  {
    let apiURL = this.Server + 'tools/find';
    let body = '{"Level":"' + level +
      '","CaseSensitive":false,' +
      this.genQuery(level,
        accessionNumber,
        patientBirthDate,
        patientID,
        patientName,
        patientSex,
        studyDate,
        studyDescription,
        modalitiesInStudy) +
      ',"Limit":' + limit +
      ',"Since":' + since +'}';
    console.log(body);
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Checks if the username and password are correct and let's you then login to the Explorer
  PARAMETERS:
    -username: name of the user
    -password: its password
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public connection(username: string, password: string)
  {
    let apiURL = this.Server + 'OE2/login';
    let body = '{"username":"' + username + '", "password":"' + password + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Retrieves the permissions of a user
  PARAMETERS:
    -username: name of the user
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public postUserPermissions(username: string)
  {
    let apiURL = this.Server + 'OE2/userPermissions';
    let body = '{"username":"' + username + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Add a tag to the list of possible tags
  PARAMETERS:
    -tag: the tag to be added
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public addTag(tag: string)
  {
    let apiURL = this.Server + 'OE2/addTag';
    let body = '{"tag":"' + tag + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Binds a tag to a study
  PARAMETERS:
    -tag: the tag to be associated to the study
    -study: the id of the corresponding study
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public addStudyTag(tag: string, study: string)
  {
    let apiURL = this.Server + 'OE2/addTagStudy';
    let body = '{"tag":"' + tag + '","study":"'+ study+'"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Remove a tag from a study
  PARAMETERS:
    -tag: tag to be removed
    -study: the study from whom it should be removed
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public deleteStudyTag(tag: string, study: string)
  {
    let apiURL = this.Server + 'OE2/removeTagStudy';
    let body = '{"tag":"' + tag + '","study":"'+ study+'"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  This function adds a user to the user database.
  Parameters:
  -username: name to be assigned to the user
  -password: password to be assigned to the user
   */
  public addUser(username: string, password: string)
  {
    let apiURL = this.Server + 'OE2/addUser';
    let body = '{"username":"' + username + '", "password":"' + password + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  This function adds a permission to a user in the database.
  Parameters:
  -username: name of the user
  -permission: permission to be assigned to the user
   */
  public addPermission(username: string, permission: string)
  {
    let apiURL = this.Server + 'OE2/addPermission';
    let body = '{"username":"' + username + '", "permission":"' + permission + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Adds a remote modality in the Orthanc config file
  PARAMETERS:
    -modality: the modality to be added
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public addModality(modality: Modality) {
    let url = this.Server + 'OE2/addModalities';
    let body: string;
    if(modality.patch == undefined) {
      body = '{"name":"' + modality.name + '", "AET":"' + modality.aet + '", "IP":"' + modality.ip + '", "port":' + modality.port + ', "patch":"undefined"}';
    }
    else {
      body = '{"name":"' + modality.name + '", "AET":"' + modality.aet + '", "IP":"' + modality.ip + '", "port":' + modality.port + ', "patch":"' + modality.patch + '"}';
    }
    return this.http.post(url, body).toPromise();
  }

  /*
  Removes a remote modality from Orthanc
  PARAMETERS:
    -name: name of the modality
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public removeModality(name: string) {
    let url = this.Server + 'OE2/removeModality';
    let body = '{"name":"' + name + '"}';
    return this.http.post(url, body).toPromise();
  }

  /*
  Sends an element (patient, study, series or instance) to a remote modality
  PARAMETERS:
    -modality: name of the remote modality
    -elemID: ID of the element
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public sendToModality(modality: string, elemID: string) {
    let url = this.Server + 'modalities/' + modality + '/store';
    let body = elemID;
    return this.http.post(url, body).toPromise();
  }

  /*
  This function removes a permission from a user in the database.
  Parameters:
  -username: name of the user
  -permission: permission to be removed from the user
   */
  public removePermission(username: string, permission: string)
  {
    let apiURL = this.Server + 'OE2/removePermission';
    let body = '{"username":"' + username + '", "permission":"' + permission + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Removes a tag from the database
  PARAMETERS:
    -tag: tag to be removed
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public removeTag(tag: string) {
    let apiURL = this.Server + 'OE2/removeTag';
    let body = '{"tag":"' + tag + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Removes a user from the database
  PARAMETERS:
    -username: name of the user
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public removeUser(username: string) {
    let apiURL = this.Server + 'OE2/removeUser';
    let body = '{"username":"' + username + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Modifies the characterising elements corresponding to a remote modality
  PARAMETERS:
    -name: identifying name of the modality
    -aet: new AET of the modality
    -ip: IP address of the modality
    -port: port number under which the modality can be reached
    -patch: patch value of the modality
   RETURN:
    HTTP post response encapsulated in a promise
   */
  public modifyModality(name: string, aet: string, ip: string, port: number, patch: string) {
    let url = this.Server + 'OE2/modifyModalities';
    let body = '{"name":"' + name + '", "AET":"' + aet + '", "IP":"' + ip + '", "port":' + port + ', "patch":"' + patch + '"}';
    return this.http.post(url, body).toPromise();
  }

  /*
  Modifies the name of a remote modality
  PARAMETERS:
    -name: current name of the modality
    -nName: new name of the modality
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public modifyModalityName(name: string, nName: string) {
    let url = this.Server + 'OE2/renameModalities';
    let body = '{"name":"' + name + '", "nName":"' + nName + '"}';
    return this.http.post(url, body).toPromise();
  }

  /*
  Modifies the name given to a tag
  PARAMETERS:
    -tag: current tag name
    -nTag: new tag name
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public modifyTag(tag: string, nTag: string) {
    let apiURL = this.Server + 'OE2/modifyTag';
    let body = '{"tag":"' + tag + '", "nTag":"' + nTag + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Reinitialises a user by setting its password equal to its username
  PARAMETERS:
    -username: name of the user
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public reinitialiseUser(username: string) {
    let apiURL = this.Server + 'OE2/reinitialiseUser';
    let body = '{"username":"' + username + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Adds a plugin to the Orthanc config file
  PARAMETERS:
    -path: path to the file or the directory where the plugin is located
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public addPlugin(path: string) {
    let apiURL = this.Server + 'OE2/addPlugin';
    let body = '{"path":"' + path + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Removes a plugin from the Orthanc config file
  PARAMETERS:
    -path: path to the file or the directory where the plugin is located
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public removePlugin(path: string) {
    let apiURL = this.Server + 'OE2/removePlugin';
    let body = '{"path":"' + path + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Changes the password of the user, but only if the first password provided corresponds to the current one.
  PARAMETERS:
    -username: name of the user whose password should be changed
    -password: current password of the user
    -nPassword: new password of the user
  RETURN:
    -HTTP post response encapsulated in a promise
   */
  public changePassword(username: string, password: string, nPassword: string) {
    let apiURL = this.Server + 'OE2/modifyUser';
    let body = '{"username":"' + username + '", "password":"' + password + '", "nPassword":"' + nPassword + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Fetches the list of users from the database
  RETURN:
    HTTP get response encapsulated in a promise
   */
  public getUsernameList()
  {
    let apiURL = this.Server + 'OE2/usernameList';
    return this.http.get(apiURL).toPromise();
  }

  /*
  Gets all the studies that are attached to a specific tag
  PARAMETERS:
    -tag: tag for whom you're searching the corresponding studies
  RETURN:
    HTTP post response encapsulated in a promise
   */
  public getStudiesWithTag(tag: string) {
    let apiURL = this.Server + 'OE2/studiesFromTag';
    let body = '{"tag":"' + tag + '"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Get the list of all the defined tags from the database
  RETURN:
    HTTP get response encapsulated in a promise
   */
  public getTagsList()
  {
    let apiURL = this.Server + 'OE2/tagsList';
    return this.http.get(apiURL).toPromise();
  }

  /*
   This function return a query string
   Parameters:
   Each parameters can be used to precise the request. Please send '*' if
   no value for the parameter. The server is case insensitive.
     -accessionNumber
     -patientBirthDate => format exemple = "20171116"
     -patientID
     -patientName
     -patientSex => format 'F' femelle 'M' male 'U' unknow 'O' other
     -studyDate => format exemple = "20171116"
                                    (for particular day)
                                    "20171116-"
                                    (from a particular day until today)
                                    "20051116-20111116"
                                    (from a particular day to one another)
     -studyDescription
     -ModalitiesInStudy => format exemple: ["CR" "CT"] (array of string)
                                           ["*"] for no particular value
 */
  public genQuery(level: string,
                  accessionNumber: string,
                  patientBirthDate: string,
                  patientID: string,
                  patientName: string,
                  patientSex: string,
                  studyDate: string,
                  studyDescription: string,
                  modalitiesInStudy: string[])
  {
    let query = '"Query":{' +
      '"AccessionNumber":"' + accessionNumber + '",' +
      '"PatientBirthDate":"' + patientBirthDate + '",' +
      '"PatientID":"' + patientID + '",' +
      '"PatientName":"' + patientName + '",' +
      '"PatientSex":"' + patientSex + '",' +
      '"StudyDate":"' + studyDate + '",' +
      '"StudyDescription":"' + studyDescription + '"';
    if(modalitiesInStudy[0] != '*' && level != 'Patient')
    {
      query = query + ',"ModalitiesInStudy":"' + modalitiesInStudy[0];
      for(let i = 1; i < modalitiesInStudy.length; i++)
      {
        query = query + '\\\\' + modalitiesInStudy[i];
      }
      query = query +'"';
    }
    query = query + '}';
    return query;
  }



  /*
    This function return the query ID corresponding to the request by the
    argument
    Parameters:
      -modality: the name of the orthanc server to request
      -level: the type of returned element (actually 'Study' mandatory)
    Each next parameter can be used to precise the request. Please send '*' if
    no value for the parameter. The server is case insensitive.
      -accessionNumber
      -patientBirthDate => format exemple = "20171116"
      -patientID
      -patientName
      -patientSex => format 'F' femelle 'M' male 'U' unknow 'O' other
      -studyDate => format exemple = "20171116"
                                     (for particular day)
                                     "20171116-"
                                     (from a particular day until today)
                                     "20051116-20111116"
                                     (from a particular day to one another)
      -studyDescription
      -ModalitiesInStudy => format exemple: ["CR" "CT"] (array of string)
                                            ["*"] for no particular value
  */
  public getQueryID(modality: string,
                    level: string,
                    accessionNumber: string,
                    patientBirthDate: string,
                    patientID: string,
                    patientName: string,
                    patientSex: string,
                    studyDate: string,
                    studyDescription: string,
                    modalitiesInStudy: string[])
  {
    let apiURL = this.Server + 'modalities/' + modality + '/query';
    let body = '{"Level":"' + level + '",' +
      this.genQuery(level,
        accessionNumber,
        patientBirthDate,
        patientID,
        patientName,
        patientSex,
        studyDate,
        studyDescription,
        modalitiesInStudy) + '}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
    This function retrieves answers that match the query ID
    Parameters:
      -queryID: the query ID
      -Aet: AET of the modality that will receive the data
      -index: the index of the answer to retrive in the retreive array
              if undefined, the function retrieves all the array
  */
  public retrieveAnswer(queryID: string, Aet: string, index?: number)
  {
    let apiURL = this.Server + 'queries/' + queryID;
    if(index != undefined)
      apiURL = apiURL + '/answers/' + index.toString()
    apiURL = apiURL + '/retrieve';
    return this.http.post(apiURL, Aet).toPromise();
  }


  /*
    This function anonymize a particular element.
    Parameters:
      -level: type of element to anonymize
      -id: the id of the element to anonymize
      -newName: the the anonymized name for the patient
   */
  public anonymize( level: string, id: string, newName?: string)
  {
    let apiURL = this.Server + level + '/' + id + '/anonymize';
    let body = '{}';

    if(newName != undefined)
    {
      body = '{"Replace":{"PatientName":"'+ newName+'"}}';
    }

    return this.http.post(apiURL, body).toPromise();
  }

  /*
     This function download a archive for an particular element.
     Parameters:
       -level: type of element to download
       -ID: the ID of the element to download
       -type: the type element in the zip (folder or dicomdirectory)
   */

  public getZIP(level:string, ID:string, type:string)
  {
    let URL = this.Server + level + '/' + ID;
    if(type == 'DICOMDIR')
      window.location.href = URL + '/media';
    else if(type == 'ZIP')
      window.location.href = URL + '/archive';
  }

  /*
    This function downloads a DICOM for a particularinstance
    Parameter:
      -ID: the ID of the instance to download
  */
  public getInstancesDICOM(ID: string)
  {
    window.location.href = this.Server + 'instances/' + ID + '/file';
  }

  /*
    This function displays the Json string on screen
    Parameter:
      -ID: the ID of the instance to download
  */
  public getInstancesJson(ID: string)
  {
    window.location.href = this.Server + 'instances/' + ID + '/tags';
  }

  /*
    This function returns 1 if the patient is protected (from recycling as
    defined on Orthanc's documentation) and 0 if he is not
    Parameter:
      -patientID: the ID of the patient
  */
  public getProtected(patientID: string){
    let apiURL = this.Server + 'patients/' + patientID + '/protected';
    return this.http.get(apiURL).toPromise();
  }

  /*
    This function set the "protected" status of a patient to the value given in
    the string "setTo"
    Parameters:
      - patientID: the ID of the patient
      - setTo: the value to which we want to set the protection, should be '1'
               or '0'
  */
  public putProtected(patientID: string, setTo: string) {
    let apiURL = this.Server + 'patients/' + patientID + '/protected';
    let body = setTo;
    const headers = new Headers({
      'Content-Type': 'application/dicom'
    });
    const options = new RequestOptions({
      headers: headers
    });
    return this.http.put(apiURL, body, options).toPromise();
  }

  /*
   * This function will ask the plugin to create the database at a given location
   * @param {string} pathDb path where to create the database
   * @param {boolean} disableUsers boolean to determine if the plugin will be running userless or not
   * @param {string} username username of the superUser
   * @param {string} password password of the superUser
   */
  initDB(pathDb: string, disableUsers: boolean, username: string, password: string)
  {
    let apiURL = this.Server+"OE2/initPlugin";
    let body = '{"pathDb":"'+pathDb+
              '","disableUsers":'+disableUsers+
               ',"username":"'+username+
              '","password":"'+password+'"}';
    return this.http.post(apiURL, body).toPromise();
  }

  /*
  Asks in backend if the database containing the user-permission pairs and tags
  RETURN:
    HTTP GET response encapsulated in a promise
   */
  existsDatabase()
  {
    let apiURL = this.Server+"OE2/existDatabase";
    return this.http.get(apiURL).toPromise();
  }

  /*
  Asks in backend if the permission-based account system is enabled or not
  RETURN:
    HTTP GET response encapsulated in a promise
   */
  allowUsers()
  {
    let apiURL = this.Server+"OE2/disableUsers";
    return this.http.get(apiURL).toPromise();
  }
 }
