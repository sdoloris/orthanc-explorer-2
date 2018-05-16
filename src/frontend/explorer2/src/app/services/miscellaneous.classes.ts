
export class  Patient
{
  public ID: string;
  public IsStable: boolean;
  public LastUpdate: Date;
  public MainDicomTags;
  public Studies: string[];
  public Type: string;

  constructor(pPatient: Patient)
  {
    this.ID = pPatient.ID;
    this.IsStable = pPatient.IsStable;
    this.LastUpdate = pPatient.LastUpdate;
    this.MainDicomTags = pPatient.MainDicomTags;
    this.Studies = pPatient.Studies;
    this.Type = pPatient.Type;
  }

}


export class Study
{
  public ID: string;
  public IsStable: boolean;
  public LastUpdate: Date;
  public MainDicomTags;
  public tags: string[];
  public Series: string[];
  public ParentPatient: string;
  public Type: string;
  constructor(pStudy: Study)
  {
    this.ID = pStudy.ID ;
    this.IsStable = pStudy.IsStable;
    this.LastUpdate = pStudy.LastUpdate;
    this.MainDicomTags = pStudy.MainDicomTags;
    this.Series = pStudy.Series;
    this.ParentPatient = pStudy.ParentPatient;
    this.Type=pStudy.Type;
    this.tags = [];
  }
}

export class Serie
{
  public ID: string;
  public IsStable: boolean;
  public LastUpdate: Date;
  public ExpectedNumberOfInstances: number;
  public Instances: string[];
  public MainDicomTags;
  public ParentStudy: string;
  public Type: string;

  constructor(pSeries: Serie)
  {
    this.ID = pSeries.ID;
    this.IsStable = pSeries.IsStable;
    this.LastUpdate = pSeries.LastUpdate;
    this.ExpectedNumberOfInstances = pSeries.ExpectedNumberOfInstances;
    this.Instances = pSeries.Instances;
    this.MainDicomTags = pSeries.MainDicomTags;
    this.ParentStudy = pSeries.ParentStudy;
    this.Type=pSeries.Type;
  }
}

export class Instance
{
  public ID: string;
  public FileSize: number;
  public FileUuid: string;
  public IndexInSeries: number;
  public ParentSeries: string;
  public MainDicomTags;
  public Type:string;

  constructor(pInstance: Instance)
  {
    this.ID = pInstance.ID;
    this.FileSize = pInstance.FileSize;
    this.FileUuid = pInstance.FileUuid;
    this.IndexInSeries = pInstance.IndexInSeries;
    this.ParentSeries = pInstance.ParentSeries;
    this.MainDicomTags = pInstance.MainDicomTags;
    this.Type=pInstance.Type;

  }
}

export class InstanceTags
{
  public tags;

  constructor(pInstance: Object)
  {
    this.tags=pInstance;
  }
}

export class Plugin
{
  public ID: string;
  public RootUri: string;
  public Description: string;
  public Version: string;
  public ExtendsOrthancExplorer: boolean;

  constructor(input, type: string)
  {
    if(type == 'Plugin') {
      this.ID = input.ID;
      this.RootUri = input.RootUri;
      this.Description = input.Description;
      this.Version = input.Version;
      this.ExtendsOrthancExplorer = input.ExtendsOrthancExplorer;
    }
    else if(type == 'string') {
      this.ID = input;
      this.RootUri = undefined;
      this.Description = undefined;
      this.Version = undefined;
      this.ExtendsOrthancExplorer = false;
    }
    else {
      console.error('Unable to create Plugin object. The given type is unknown.');
    }
  }
}

export class RemainingAncestor
{
  public ID: string;
  public Path: string;
  public Type: string;

  constructor(pRemainingAncestor :RemainingAncestor)
  {
    this.ID = pRemainingAncestor.ID;
    this.Path = pRemainingAncestor.Path;
    this.Type = pRemainingAncestor.Type;
  }
}

export class Statistics
{
  public CountInstances: number;
  public CountPatients: number;
  public CountSeries: number;
  public CountStudies: number;
  public TotalDiskSize: string;
  public TotalDiskSizeMB: number;
  public TotalUncompressedSize: string;
  public TotalUncompressedSizeMB: number;

  constructor (pStatistics: Statistics)
  {
    this.CountInstances = pStatistics.CountInstances;
    this.CountPatients = pStatistics.CountPatients;
    this.CountSeries = pStatistics.CountSeries;
    this.CountStudies = pStatistics.CountStudies;
    this.TotalDiskSize = pStatistics.TotalDiskSize;
    this.TotalDiskSizeMB = pStatistics.TotalDiskSizeMB;
    this.TotalUncompressedSize = pStatistics.TotalUncompressedSize;
    this.TotalUncompressedSizeMB = pStatistics.TotalUncompressedSizeMB;
  }
}

export class UploadInfo {
  public ID: string;
  public Path: string;
  public Status: string

  constructor(pUploadInfo: UploadInfo)
  {
    this.ID = pUploadInfo.ID;
    this.Path = pUploadInfo.Path;
    this.Status = pUploadInfo.Status;
  }
}


export class System
{
  public DatabaseBackendPlugin;
  public DatabaseVersion: number;
  public DicomAet: string;
  public DicomPort: number;
  public HttpPort: number;
  public Name: string;
  public PluginsEnabled: boolean;
  public StorageAreaPlugin;
  public Version: string;

  constructor(pSystem: System)
  {
    this.DatabaseBackendPlugin = pSystem.DatabaseBackendPlugin;
    this.DatabaseVersion = pSystem.DatabaseVersion;
    this.DicomAet = pSystem.DicomAet;
    this.DicomPort = pSystem.DicomPort;
    this.HttpPort = pSystem.HttpPort;
    this.Name = pSystem.Name;
    this.PluginsEnabled = pSystem.PluginsEnabled;
    this.StorageAreaPlugin = pSystem.StorageAreaPlugin;
    this.Version = pSystem.Version;
  }
}

export class Answer
{
  public AccessionNumber: string;
  public PatientBirthDate: string;
  public PatientID: string;
  public PatientName: string;
  public PatientSex: string;
  public QueryRetrieveLevel: string;
  public SpecificCharacterSet: string;
  public StudyDate: string;
  public StudyDescription: string;
  public StudyInstanceUID: string;
  constructor(pAnswer: Answer)
  {
    this.AccessionNumber = pAnswer.AccessionNumber;
    this.PatientBirthDate = pAnswer.PatientBirthDate;
    this.PatientID = pAnswer.PatientID;
    this.PatientName = pAnswer.PatientName;
    this.PatientSex = pAnswer.PatientSex;
    this.QueryRetrieveLevel = pAnswer.QueryRetrieveLevel;
    this.SpecificCharacterSet = pAnswer.SpecificCharacterSet;
    this.StudyDate = pAnswer.StudyDate;
    this.StudyDescription = pAnswer.StudyDescription;
    this.StudyInstanceUID = pAnswer.StudyInstanceUID;
  }
}

export class Modality {
  public name: string;
  public aet: string;
  public ip: string;
  public port: number;
  public patch: string;
  constructor(name: string, aet: string, ip: string, port: number, patch: string) {
    this.name = name;
    this.aet = aet;
    this.ip = ip;
    this.port = port;
    this.patch = patch;
  }
}

export class User {
  public name: string;
  public password: string;
  public permissions: string[];
  constructor(name: string, permissions: string[]) {
    this.name = name;
    this.permissions = permissions;
  }
}
