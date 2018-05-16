import {Component, Input, OnInit} from "@angular/core";
import {RESTService} from "../../../services/rest.service";
import {PermissionsService} from "../../../services/permissions.service";

@Component
({
  selector: 'tags',
  templateUrl: 'tags.component.html'
})

export class TagsComponent implements OnInit
{

  @Input() ID: string;
  @Input() selectedTag: string;
  @Input() toDeleteTag: string;

  tags: string[];
  studyTags: string[];

  constructor(private service: RESTService, private permissions: PermissionsService){}

  /*
  Called on the initialisation of the component.
   */
  ngOnInit(): void
  {
    this.getStudyTags();
    this.getAllTags()
  }

  /*
  Fetches all the tags that can be put on studies.
   */
  getAllTags()
  {
    this.service.getTagsList()
      .then(res => {
        this.tags = [];
        if(res.json() != null) {
          this.tags = res.json().tags
        }
      });
  }

  /*
  Get the tags corresponding to the study.
   */
  getStudyTags()
  {
    this.service.getTags(this.ID)
      .then(res => {
        this.studyTags = [];
        if(res.json() != null) {
          this.studyTags = res.json().tags
        }
      });
  }

  /*
  Add a tag to the study.
   */
  addTag()
  {
    this.service.addStudyTag(this.selectedTag, this.ID)
  }

  /*
  Delete a tag from the study.
   */
  deleteTag()
  {
    this.service.deleteStudyTag(this.toDeleteTag, this.ID)
  }
}
