import {Component, Input} from "@angular/core";
import {ActivatedRoute, Router} from "@angular/router";
import {TranslateService} from "@ngx-translate/core";
import {RESTService} from "../../../services/rest.service";
import {PermissionsService} from "../../../services/permissions.service";
import {Instance} from "../../../services/miscellaneous.classes";


@Component
({
  selector: 'preview',
  templateUrl: 'preview.component.html',
  providers: [RESTService]
})

export class PreviewComponent
{
  @Input() id: string;
  image: string;
  load: boolean = false;

  constructor(private service: RESTService, private router: Router, private route: ActivatedRoute, private permissions: PermissionsService,
              private translateService: TranslateService)
  {
  }

  /*
  Called on the initialisation of the component
   */
  ngOnInit(): void
  {
  }

  /*
  Show a preview of a DICOM file
   */
  show() {
    this.load = true;
  }
}
