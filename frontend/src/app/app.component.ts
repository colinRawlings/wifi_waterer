import { Component, OnDestroy, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { ConstantsService } from './constants.service';
import { NotifierService } from 'angular-notifier';
import { PumpSettingsService } from './pump-settings.service';
import { PumpStatusService } from './pump-status.service';

import packageInfo from '../../package.json';
import { Subscription } from 'rxjs';

interface keyable {
  [key: string]: any;
}
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
})
export class AppComponent implements OnInit, OnDestroy {

  private constantsIsReadySubscription: Subscription;
  private statusIsReadySubscription: Subscription;
  private statusSubscriptions: Subscription[];
  private settingsIsReadySubscription: Subscription;
  private settingsSubscriptions: Subscription[];

  //

  title = 'waterer';
  public autoUpdate: boolean;
  public autoSwitchGraphs: boolean;
  public channels: number[] = [];
  public channelConnected: boolean[] = [];
  public channelName: string[] = [];
  public numChannels: number = 0;

  public devices: string[];
  public frontend_version: string = packageInfo.version;
  public backend_version: string;

  constructor(
    private http: HttpClient,
    private notifierService: NotifierService,
    private constantsService: ConstantsService,
    private statusService: PumpStatusService,
    private settingsService: PumpSettingsService,
  ) {
    this.devices = [];
    this.autoUpdate = true;
  }

  ngOnInit(): void {

    console.log(`ngOnInit App Component`);

    if (this.constantsService.isReady) {
      this.Init();
    } else {
      this.constantsIsReadySubscription = this.constantsService.isReady$.subscribe((constantsIsReady: boolean) => {
        if (!constantsIsReady) console.error("Constants notified not-ready");
        this.constantsIsReadySubscription.unsubscribe();
        this.Init();
      })
      this.constantsService.Init();
    }
  }

  public Init(): void {
    this.channels = [];
    this.channelConnected = [];
    this.statusSubscriptions = [];
    this.settingsSubscriptions = [];
    for (let channel = 0; channel < this.constantsService.numChannels; channel++) {
      this.channels.push(channel);
      this.channelConnected.push(false);
      this.channelName.push(channel.toString());
    }

    this.http.get(this.constantsService.kBackendURL).subscribe(
      (data: keyable) => {
        this.devices = data.device_info;
        this.backend_version = `${data.version}`;

        this.PreInitStatus();
        this.PreInitSettings();
      },
      (err) => this.notifierService.notify('error', `HTTP Error:  ${err.message}`)
    );
  }

  ngOnDestroy(): void {
    this.statusService.stopDataStream();
  }

  PreInitStatus(): void {
    if (this.statusService.isReady) {
      this.InitStatus();
      this.onAutoUpdateChange();
    } else {
      this.statusIsReadySubscription = this.statusService.isReady$.subscribe((statusIsReady: boolean) => {
        if (!statusIsReady) console.error("Status notified not-ready");
        this.statusIsReadySubscription.unsubscribe();
        this.onAutoUpdateChange();
        this.InitStatus();
      })
      this.statusService.Init();
    }
  }

  PreInitSettings(): void {
    if (this.settingsService.isReady) {
      this.InitSettings();
      this.onAutoUpdateChange();
    } else {
      this.settingsIsReadySubscription = this.settingsService.isReady$.subscribe((settingsIsReady: boolean) => {
        if (!settingsIsReady) console.error("Settings notified not-ready");
        this.settingsIsReadySubscription.unsubscribe();
        this.onAutoUpdateChange();
        this.InitSettings();
      })
      this.settingsService.Init();
    }
  }

  private InitStatus(): void {
    for (let channel = 0; channel < this.constantsService.numChannels; channel++) {
      this.statusSubscriptions.push(this.statusService.allStatuses[channel].subscribe((data: keyable) => {
        this.channelConnected[channel] = data.data.is_connected;
      }))
    };
  }

  private InitSettings(): void {
    for (let channel = 0; channel < this.constantsService.numChannels; channel++) {
      this.settingsSubscriptions.push(this.settingsService.allSettings$[channel].subscribe((data: keyable) => {
        this.channelName[channel] = data.data.name;
      },
      (error: keyable) => {
        this.notifierService.notify('error', `app set settings Error:  ${error.message}`);
      }
      ))
      this.settingsService.updateSettings(channel);
    };
  }

  onAutoUpdateChange(): void {
    if (this.autoUpdate) {
      this.statusService.startDataStream();
    } else {
      this.statusService.stopDataStream();
    }
  }

  onSaveHistory(): void {
    this.http.get(`${this.constantsService.kBackendURL}save_history`).subscribe(
      (data: keyable) => {
        this.notifierService.notify('success', `Saved history to ${data.data}`);
      },
      (err) => this.notifierService.notify('error', `HTTP Error:  ${err.message}`)
    );
  }
}
