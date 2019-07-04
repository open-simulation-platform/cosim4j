pipeline {
    agent none

    stages {

        stage('Build') {
            parallel {
                /*stage('Windows') {
                    agent { label 'windows' }
                    
                    steps {
                        bat 'gradlew.bat clean build'
                    }
                }*/

                stage ('Linux') {
                    agent { 
                        dockerfile {
                            filename 'Dockerfile.build'
                            dir '.dockerfiles'
                            label 'linux && docker'
                        }
                    }

                    steps {
                        sh './cse-core4j/gradlew clean build'
                    }
                }
            }
        }
    }
}
