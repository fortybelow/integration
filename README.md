## Jenkins Integration with Github
### Pipeline
- Using Generic Webhook Plugin.
- Configure this for use with pipeline job via __*Build Triggers*__.
- Ensure Jenkins instance has appropriate Pipeline plugins. (__*Pipeline: Declarative*__, etc.)
- Build Job Twice for good measure.
- Send webhooks to JENKINS_URL/generic-webhook-trigger/invoke?token=TOKENVALUE

### fin.    
