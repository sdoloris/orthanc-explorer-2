// TODO: avoid redundancy and comment the code


import {
  fakeAsync,
  inject,
  TestBed
} from '@angular/core/testing';
import {
  HttpModule,
  XHRBackend,
  ResponseOptions,
  Response,
  RequestMethod
} from '@angular/http';
import {
  MockBackend,
  MockConnection
} from '@angular/http/testing/';

import {RESTService} from './rest.service';
import {Patient, Study, Serie, Instance, Plugin, RemainingAncestor, Statistics, UploadInfo, System, Answer, Modality, User} from './miscellaneous.classes';
const mockResponse1 =  {
  "Description" : "Serve additional folders with the HTTP server of Orthanc.",
  "ExtendsOrthancExplorer" : false,
  "ID" : "serve-folders",
  "RootUri" : "../app/plugin-serve-folders.html",
  "Version" : "1.2.0"
};
const mockResponse2 ={
  "DatabaseBackendPlugin" : null,
  "DatabaseVersion" : 6,
  "DicomAet" : "ORTHANC",
  "DicomPort" : 4242,
  "HttpPort" : 8042,
  "Name" : "",
  "PluginsEnabled" : true,
  "StorageAreaPlugin" : null,
  "Version" : "1.2.0"
};
const mockResponse3 =[
{
  "ID" : "b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c",
  "IsStable" : true,
  "LastUpdate" : "20171120T112841",
  "MainDicomTags" : {
    "PatientID" : "0",
    "PatientName" : "Anonymized"
  },
  "Studies" : [ "3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12" ],
  "Type" : "Patient"
}
];

const mockResponse4 =[
{
  "ID" : "3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12",
  "IsStable" : true,
  "LastUpdate" : "20171120T112841",
  "MainDicomTags" : {
    "StudyDate" : "20061012",
    "StudyDescription" : "CT1 abdomen",
    "StudyInstanceUID" : "1.2.826.0.1.3680043.8.1055.1.20111102150758591.92402465.76095170",
    "StudyTime" : "090258.000000"
  },
  "ParentPatient" : "b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c",
  "PatientMainDicomTags" : {
    "PatientID" : "0",
    "PatientName" : "Anonymized"
  },
  "Series" : [ "98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2" ],
  "Type" : "Study"
}
];

const mockResponse5 =[
{
  "ExpectedNumberOfInstances" : 2,
  "ID" : "98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2",
  "Instances" : [
  "d239b09a-b1045d14-af935ad9-eb2483c9-1b63618d",
  "82e45f33-802dad2e-c3855875-a41ab2c1-d42d70a9"
  ],
  "IsStable" : true,
  "LastUpdate" : "20171120T112841",
  "MainDicomTags" : {
    "ContrastBolusAgent" : "CONTRAST",
    "ImageOrientationPatient" : "1\\0\\0\\0\\1\\0",
    "Modality" : "CT",
    "PerformedProcedureStepDescription" : "CT1 abdomen",
    "ProtocolName" : "ART.RENALES 12/Abdomen/Hx",
    "SeriesDescription" : "ARTERIELLE",
    "SeriesInstanceUID" : "1.2.826.0.1.3680043.8.1055.1.20111102150758591.96842950.07877442",
    "SeriesNumber" : "6168"
  },
  "ParentStudy" : "3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12",
  "Status" : "Unknown",
  "Type" : "Series"
}
];

const mockResponse6 =[

{
  "FileSize" : 92352,
  "FileUuid" : "3AD6E106-B2F8-4467-8E3B-9A516616DF46",
  "ID" : "d239b09a-b1045d14-af935ad9-eb2483c9-1b63618d",
  "IndexInSeries" : 196,
  "MainDicomTags" : {
    "ImageComments" : "JPEG 2000 lossless - Version 4.0.2 (c) Image Devices GmbH",
    "ImagePositionPatient" : "-151.493508\\-36.6564417\\1197.5",
    "InstanceCreationDate" : "20061012",
    "InstanceCreationTime" : "091656.000000",
    "InstanceNumber" : "196",
    "SOPInstanceUID" : "1.2.826.0.1.3680043.8.1055.1.20111102150832466.64153593.61884440"
  },
  "ParentSeries" : "98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2",
  "Type" : "Instance"
},
{
  "FileSize" : 93428,
  "FileUuid" : "A8271D00-71EF-41ED-9008-28347388B29E",
  "ID" : "82e45f33-802dad2e-c3855875-a41ab2c1-d42d70a9",
  "IndexInSeries" : 199,
  "MainDicomTags" : {
    "ImageComments" : "JPEG 2000 lossless - Version 4.0.2 (c) Image Devices GmbH",
    "ImagePositionPatient" : "-151.493508\\-36.6564417\\1196",
    "InstanceCreationDate" : "20061012",
    "InstanceCreationTime" : "091658.000000",
    "InstanceNumber" : "199",
    "SOPInstanceUID" : "1.2.826.0.1.3680043.8.1055.1.20111102150832669.59350116.41748365"
  },
  "ParentSeries" : "98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2",
  "Type" : "Instance"
}];
const mockResponse7 =[ "explorer.js", "osimis-web-viewer", "serve-folders" ];

const mockResponse8 =
{
  "ID" : "b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c",
  "IsStable" : true,
  "LastUpdate" : "20171120T112841",
  "MainDicomTags" : {
    "PatientID" : "0",
    "PatientName" : "Anonymized"
  },
  "Studies" : [ "3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12" ],
  "Type" : "Patient"
};

const mockResponse9 =
{
  "ID" : "3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12",
  "IsStable" : true,
  "LastUpdate" : "20171120T112841",
  "MainDicomTags" : {
    "StudyDate" : "20061012",
    "StudyDescription" : "CT1 abdomen",
    "StudyInstanceUID" : "1.2.826.0.1.3680043.8.1055.1.20111102150758591.92402465.76095170",
    "StudyTime" : "090258.000000"
  },
  "ParentPatient" : "b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c",
  "PatientMainDicomTags" : {
    "PatientID" : "0",
    "PatientName" : "Anonymized"
  },
  "Series" : [ "98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2" ],
  "Type" : "Study"
};
const mockResponse10={
   "CountInstances" : 261,
   "CountPatients" : 1,
   "CountSeries" : 1,
   "CountStudies" : 1,
   "TotalDiskSize" : "27009193",
   "TotalDiskSizeMB" : 25,
   "TotalUncompressedSize" : "27009193",
   "TotalUncompressedSizeMB" : 25
};
const mockResponse11=[ {"aet":"ORTHANCB","ip":"127.0.0.1","name":"orthanc-b","port":4243}];

const mockResponse12={
   "ExpectedNumberOfInstances" : null,
   "ID" : "37836232-d13a2350-fa1dedc5-962b31aa-010f8e52",
   "Instances" : [ "6582b1c0-292ad5ab-ba0f088f-f7a1766f-9a29a54f" ],
   "IsStable" : true,
   "LastUpdate" : "20171204T154935",
   "MainDicomTags" : {
      "ImageOrientationPatient" : "0\\1\\0\\0\\0\\-1",
      "Manufacturer" : "Philips",
      "Modality" : "CT",
      "PerformedProcedureStepDescription" : "CT2 tête, face, sinus",
      "ProtocolName" : "COU 2 mm/Neck/Sv",
      "SeriesInstanceUID" : "1.2.840.113704.1.111.5692.1127828999.2",
      "SeriesNumber" : "2276",
      "StationName" : "ctrad"
   },
   "ParentStudy" : "49974143-ec23cb52-6b2a1c46-14d5daa0-0822ce1a",
   "Status" : "Unknown",
   "Type" : "Series"
}
const mockResponse13={"tags":["Tester"]}

const mockResponse14={"permissions":["Read","Write","Delete","SuperUser"],"username":"root"}

const mockResponse15={"userList":["root","toor"]}

const mockResponse16 ={"studyList":["3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12"]}




describe('testing service', () => {

  beforeEach(() => {
    TestBed.configureTestingModule({
      imports: [HttpModule],
      providers: [
      {
        provide: XHRBackend,
        useClass: MockBackend
      },
      RESTService
      ]
    });
  });

  it('should get all plugin info as a results', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/plugins/serve-folders';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse1 })
              ));


          });

        RESTService.getPluginInfo('serve-folders').then(res => {
          let result =new Plugin(res.json(), 'Plugin');
          expect(result.Version).toBe('1.2.0');
          expect(result.RootUri).toBe('../app/plugin-serve-folders.html');
          expect(result.Description).toBe('Serve additional folders with the HTTP server of Orthanc.');
          expect(result.ExtendsOrthancExplorer).toBe(false);
          });
      })
    ));
   it('test getSystem info', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/system';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse2 })
              ));

          });
        RESTService.getSystem()
        .then( res=>{
          let results =new System(res.json());
          expect(results.DatabaseBackendPlugin).toBe(null);
          expect(results.DatabaseVersion).toBe(6);
          expect(results.DicomAet).toBe('ORTHANC');
          expect(results.DicomPort).toBe(4242);
          expect(results.HttpPort).toBe(8042);
          expect(results.Name).toBe('');
          expect(results.PluginsEnabled).toBe(true);
          expect(results.StorageAreaPlugin).toBe(null);
          expect(results.Version).toBe('1.2.0'); });

      })
    ));

  it('test getPatient', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/patients?&since=0&limit=1&expand';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse3 })
              ));


          });
        RESTService.getPatients(1,0)
        .then( res=>{
          let results :Patient[];
          results = res.json().map(patient => {
            return new Patient(patient);
          })
        expect(results[0].ID).toBe('b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c');
          expect(results[0].IsStable).toBe(true);
          expect(results[0].Studies[0]).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
          expect(results[0].Type).toBe('Patient'); });
      })
    ));
  it('test getStudies', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/studies?&since=0&limit=1&expand';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse4 })
              ));


          });
        RESTService.getStudies(1,0)
        .then( res=>{
            let results :Study[];
            results = res.json().map(study => {
              return new Study(study);
            })
             expect(results[0].ID).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
          expect(results[0].IsStable).toBe(true);
          expect(results[0].Series[0]).toBe('98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2');
          expect(results[0].Type).toBe('Study');
          expect(results[0].ParentPatient).toBe('b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c') });
      })
    ));

  it('test getPatientStudies  with getChildren', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/patients/b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c/studies';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse4 })
              ));


          });
        RESTService.getChildren('patients','b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c')
        .then( res=>{
          let results :Study[];
            results = res.json().map(study => {
              return new Study(study);
            })
            expect(results[0].ID).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
          expect(results[0].IsStable).toBe(true);
          expect(results[0].Series[0]).toBe('98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2');
          expect(results[0].Type).toBe('Study');
          expect(results[0].ParentPatient).toBe('b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c') });
      })
    ));

  it('test getStudySeries  with getChildren', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/studies/3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12/series';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse5 })
              ));


          });
        RESTService.getChildren('studies','3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12')
        .then( res=>{
          let results :Serie[];
            results = res.json().map(serie => {
          return new Serie(serie);
          });
          expect(results[0].ID).toBe('98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2');
          expect(results[0].IsStable).toBe(true);
          expect(results[0].Instances[0]).toBe('d239b09a-b1045d14-af935ad9-eb2483c9-1b63618d');
          expect(results[0].Type).toBe('Series');
          expect(results[0].ParentStudy).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
          expect(results[0].ExpectedNumberOfInstances).toBe(2) });

      })
    ));
  it('test getSeriesInstances with getChildren', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/series/98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2/instances';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse6 })
              ));

          });

        RESTService.getChildren('series','98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2')
        .then( res=>{
          let results :Instance[];
            results = res.json().map(instance => {
          return new Instance(instance);
          })
          expect(results[1].FileSize).toBe(93428);
          expect(results[1].ID).toBe('82e45f33-802dad2e-c3855875-a41ab2c1-d42d70a9');
          expect(results[1].FileUuid).toBe('A8271D00-71EF-41ED-9008-28347388B29E');
          expect(results[1].IndexInSeries).toBe(199);
          expect(results[1].Type).toBe('Instance');
          expect(results[1].ParentSeries).toBe('98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2'); });

      })
    ));

  it('test getPluginsName', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/plugins';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse7 })
              ));


          });
        RESTService.getPluginsName()
        .then( res=>{
          let results=res.json()
        .map(name => {
          return name;
        });
          expect(results[0]).toBe('explorer.js');
          expect(results[1]).toBe('osimis-web-viewer');
          expect(results[2]).toBe('serve-folders');});

      })
    ));

  it('test getPatientInfo', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/patients/b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse8 })
              ));


          });
        RESTService.getPatientInfo('b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c')
        .then( res=>{
          let results =new Patient(res.json());
          expect(results.ID).toBe('b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c');
          expect(results.IsStable).toBe(true);
          expect(results.Studies[0]).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
          expect(results.Type).toBe('Patient');});

      })
    ));
  it('test getStudiesInfo', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/studies/3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse9 })
              ));


          });
        RESTService.getStudyInfo('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12')
        .then( res=>{
          let results =new Study(res.json());
          expect(results.ID).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
          expect(results.IsStable).toBe(true);
          expect(results.Series[0]).toBe('98d367a5-58eeba8b-f3c2da4b-de7d8f6f-ede53eb2');
          expect(results.Type).toBe('Study');
          expect(results.ParentPatient).toBe('b6589fc6-ab0dc82c-f12099d1-c2d40ab9-94e8410c') });
      })
    ));

  it('test getSeriesInfo', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/series/37836232-d13a2350-fa1dedc5-962b31aa-010f8e52';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse12 })
              ));


          });
        RESTService.getSerieInfo('37836232-d13a2350-fa1dedc5-962b31aa-010f8e52')
        .then( res=>{
          let results =new Serie(res.json());
          expect(results.ID).toBe('37836232-d13a2350-fa1dedc5-962b31aa-010f8e52');
          expect(results.IsStable).toBe(true);
          expect(results.Instances[0]).toBe('6582b1c0-292ad5ab-ba0f088f-f7a1766f-9a29a54f');
          expect(results.Type).toBe('Series');
          expect(results.ParentStudy).toBe('49974143-ec23cb52-6b2a1c46-14d5daa0-0822ce1a') });
      })
    ));




it('test getStatistics', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/statistics';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse10 })
              ));


          });
        RESTService.getStatistics()
        .then( res=>{
          let results = new Statistics(res.json());
          expect(results.CountInstances).toBe(261);
          expect(results.CountPatients).toBe(1);
          expect(results.CountSeries).toBe(1);
          expect(results.CountStudies).toBe(1);
          expect(results.TotalDiskSize).toBe('27009193');
          expect(results.TotalDiskSizeMB).toBe(25)
          expect(results.TotalUncompressedSize).toBe('27009193')
          expect(results.TotalUncompressedSizeMB).toBe(25) });
      })
    ));
it('test getModalities', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/OE2/modalities';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse11 })
              ));


          });
        RESTService.getModalities()
        .then( res=>{
          let results: Modality[];
          results = res.json()
          .map(modality => {
            return new Modality(modality.name, modality.aet, modality.ip, modality.port, modality.patch);
          })
          expect(results[0].name).toBe('orthanc-b');
          expect(results[0].aet).toBe('ORTHANCB');
          expect(results[0].ip).toBe('127.0.0.1');
          expect(results[0].port).toBe(4243);
           });
      })
    ));
it('test Userdata', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/OE2/userdata';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse14 })
              ));


          });
        RESTService.getPermission()
        .then( res=>{
          let results= res.json();
          expect(results.username).toBe('root');
          expect(results.permissions[0]).toBe('Read');
          expect(results.permissions[1]).toBe('Write');
          expect(results.permissions[2]).toBe('Delete');
          expect(results.permissions[3]).toBe('SuperUser');
           });
      })
    ));


it('test Userlist', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/OE2/usernameList';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse15 })
              ));


          });
        RESTService.getUsernameList()
        .then( res=>{
          let results=[];
          results= res.json().userList;
          expect(results[0]).toBe('root');
          expect(results[1]).toBe('toor');
           });
      })
    ));
it('test getTags', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/OE2/tags/3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse13 })
              ));


          });
        RESTService.getTags('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12')
        .then( res=>{
          let tags:string[];
             tags= res.json().tags;
          expect(tags[0]).toBe('Tester');
           });
      })
    ));
it('test getTaglist', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/OE2/tagsList';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Get);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse13 })
              ));


          });
        RESTService.getTagsList()
        .then( res=>{
          let tags:string[];
          tags= res.json().tags;
          expect(tags[0]).toBe('Tester');
           });
      })
    ));

it('test getStudiesWithTag', fakeAsync(
    inject([
      XHRBackend,
      RESTService
      ], (mockBackend: MockBackend, RESTService: RESTService) => {

        const expectedUrl = 'http://localhost:8042/OE2/studiesFromTag';

        mockBackend.connections.subscribe(
          (connection: MockConnection) => {
            expect(connection.request.method).toBe(RequestMethod.Post);
            expect(connection.request.url).toBe(expectedUrl);

            connection.mockRespond(new Response(
              new ResponseOptions({ body: mockResponse16 })
              ));


          });

      RESTService.getStudiesWithTag("tag")
        .then(res => {
          if(res.json() != null) {
            let studies: string[];
            studies = res.json().studyList;
            expect(studies[0]).toBe('3abaec0a-ebfaaa87-f830d52b-d62df074-6a692c12');
           }});
      })
    ));


});
