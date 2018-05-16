import {Component, Input} from "@angular/core";
import {ActivatedRoute, Router} from "@angular/router";
import {TranslateService} from "@ngx-translate/core";
import {RESTService} from "../../../services/rest.service";
import {PermissionsService} from "../../../services/permissions.service";


@Component
({
  selector: 'tags-modal',
  templateUrl: 'tags-modal.component.html',
  providers: [RESTService]
})

export class TagsModalComponent
{
  @Input() studyID: string;
  allTags: string[];
  studyTags: string[];


  constructor(private service: RESTService, private router: Router, private route: ActivatedRoute, private permissions: PermissionsService,
              private translateService: TranslateService)
  {
    this.allTags = [];
    this.studyTags = [];
  }

  /*
  Called on the initialisation of the component.
   */
  ngOnInit(): void
  {
    this.getStudyTags();
    this.getAllTags();
  }

  /*
  Gets all the different tags that can be put on studies.
   */
  getAllTags()
  {
    this.service.getTagsList()
      .then(res => {
        let resJson = res.json();
        if (resJson != null) {
          this.allTags = resJson.tags;
        }
      });

  }

  /*
  Gets the studies associated to the study.
   */
  getStudyTags()
  {
    this.service.getTags(this.studyID)
      .then(res => {
        let resJson = res.json();
        if (resJson != null) {
          this.studyTags = resJson.tags;

        }
      });
  }

  /*
  Verifies if the study is attached to the specified tag.
  PARAMETERS:
    -tag: tag to be verified
  RETURN:
    boolean indicating whether or not the tag is attached to it
   */
  studyHasTag(tag: string) : boolean
  {
    return this.studyTags.includes(tag);
  }

  /*
  Add or remove the attachment of a tag to a study.
  PARAMETER:
    -tag: tag to be removed or added
   */
  tagToStudy(tag: string)
  {
    if (this.studyHasTag(tag)) {
      this.service.deleteStudyTag(tag, this.studyID).then(
        update => {
          this.studyTags.splice(this.studyTags.indexOf(tag), 1);
        }
      )
    }
    else {
      this.service.addStudyTag(tag, this.studyID).then(
        update => {
          this.studyTags.splice(0,0,tag);
        }
      );
    }
  }
}
