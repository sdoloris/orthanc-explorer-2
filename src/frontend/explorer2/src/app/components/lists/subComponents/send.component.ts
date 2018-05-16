//Component used to send to a remote modality
import {Component, Input} from "@angular/core";
import {RESTService} from "../../../services/rest.service";
import {PermissionsService} from "../../../services/permissions.service";
import {TranslateService} from "@ngx-translate/core";
import {Modality} from "../../../services/miscellaneous.classes";

@Component
({
  selector: 'send',
  templateUrl: 'send.component.html',
  providers: [RESTService]
})

export class SendComponent {
  @Input() ID: string;
  modalities: Modality[];

  constructor(private permissions: PermissionsService, private restService: RESTService,
              private translateService: TranslateService) {
    this.modalities = [];
    this.restService.getModalities()
      .then(res => {
        if(res.json() != null) {
          this.modalities = res.json()
            .map(modality => {
              return new Modality(modality.name, modality.aet, modality.ip, modality.port, modality.patch);
            })
        }
      })
  }

  /*
  Sends the element corresponding to the ID to a remote modality
  PARAMETERS:
    -modality: name of the remote modality
   */
  sendToModality(modality: string): void {
    console.log("Sending to " + modality + "...");
    this.restService.sendToModality(modality, this.ID)
      .then( res => {
        if(res.status == 200) {
          console.log("File successfully sent to remote modality.");
          document.getElementById('selectModality').style.display = "none";
        }
        else {
          console.error("Unable to send the file to the remote modality." +
                        "\nError status: " + res.status + "\n" + res.statusText);
        }
      })
  }

  /*
  Makes the division in the html document where you can choose the remote modality to send to visible.
   */
  showSelectModality() {
    document.getElementById('selectModality').style.display = "block";
  }

}
