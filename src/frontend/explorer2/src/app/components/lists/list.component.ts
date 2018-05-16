export abstract class ListComponent
{
  listeType: string;
  isSearch: boolean;
  searchParameters: string[];
  nbInstances: number;

  constructor() {}


  /*
  Get the type of the list (patients, studies, series or instances list).
  RETURN:
    string indicating the type of the list
   */
  getListeType(): string
  {
    return this.listeType;
  }

  /*
  Get the number of instances in the list component.
  RETURN:
    Integer number indicating the number of instances
   */
  getNbInstances(): number
  {
    return this.nbInstances;
  }

  /*
  Returns the elements in the list in the given interval.
  PARAMETERS:
    -nb: number of elements to be retrieved
    -from: the index of the first element to retrieve
   */
  abstract getElements(nb: number, from: number): void;

}
