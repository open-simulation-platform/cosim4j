pipeline {
    agent none

    stages {

        stage('Build') {
            parallel {
                stage('Windows') {
                    agent { label 'windows' }

                    tools { jdk 'jdk8' }
                    
                    steps {
                        bat 'gradlew.bat clean build'
                    }
                }

                stage ('Linux') {
                    agent { 
                        dockerfile {
                            filename 'Dockerfile.build'
                            dir '.dockerfiles'
                            label 'linux && docker'
                        }
                    }

                    steps {
                        sh './gradlew clean build'
                    }
                }
            }
        }
    }
}
