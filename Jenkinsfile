pipeline {
    agent none

    stages {

        stage('Build') {
            parallel {
                stage('Windows') {
                    agent { label 'windows' }
                    
                    steps {
                        bat 'gradlew.bat clean build'
                    }
                }

                stage ('Linux') {
                    agent { 
                        dockerfile {
                            filename 'Dockerfile.build'
                            dir 'cse-core/.dockerfiles'
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
        